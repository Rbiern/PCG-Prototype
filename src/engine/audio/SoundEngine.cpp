#include "SoundEngine.hpp"

#include "AudioManifestLoader.hpp"
#include <SFML/Audio/InputSoundFile.hpp>
#include "core/ApplicationSettings.hpp"
#include <SFML/Audio/Listener.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <utility>
#include <vector>


namespace {
    constexpr float WorldUnitsPerMeter = 64.f;
    constexpr std::size_t MaxSoundInstances = 64;
    constexpr std::size_t MaxRequests = 256;

    bool finite(sf::Vector2f position) {
        return std::isfinite(position.x) && std::isfinite(position.y);
    }

    float duration(float seconds) {
        return std::isfinite(seconds) ? std::max(0.f, seconds) : 0.f;
    }

    float channelGain(const AudioChannelSettings& channel) {
        return channel.enabled ? std::min(channel.level, 100u) / 100.f : 0.f;
    }
}


SoundEngine::SoundEngine(const AudioSettings& audioSettings)
    : audioSettings(audioSettings) {
}


SoundEngine::~SoundEngine() {
    stopAll();
    for (auto& [id, resource] : sounds) {
        delete resource.buffer;
    }
}


bool SoundEngine::loadManifest(const std::filesystem::path& manifestPath) {
    if (loaded) {
        return true;
    }

    std::string error;
    AudioManifestLoader loader(manifestPath);
    if (!loader.load(error)) {
        std::cerr << "Audio: " << error << std::endl;
        return false;
    }

    std::unordered_map<std::string, PreparedSound> preparedSounds;
    std::unordered_map<std::string, PreparedMusic> preparedMusic;
    sf::SoundBuffer* pendingBuffer = nullptr;
    const auto cleanup = [&] {
        delete pendingBuffer;
        for (auto& [id, resource] : preparedSounds) {
            delete resource.buffer;
        }
    };
    try {
        for (const auto& [id, def] : loader.catalog().sounds) {
            pendingBuffer = new sf::SoundBuffer;

            if (!pendingBuffer->loadFromFile(def.file)) {
                std::cerr << "Audio: cannot decode sound; skipped: " << def.file << std::endl;
                delete pendingBuffer;
                pendingBuffer = nullptr;
                continue;
            }
            if (def.spatial && pendingBuffer->getChannelCount() != 1) {
                std::cerr << "Audio: spatial sound must be mono; skipped: " << def.file << std::endl;
                delete pendingBuffer;
                pendingBuffer = nullptr;
                continue;
            }

            const auto result = preparedSounds.emplace(id, PreparedSound{def, pendingBuffer});
            if (!result.second) {
                delete pendingBuffer;
            }

            pendingBuffer = nullptr;
        }

        for (const auto& [id, def] : loader.catalog().music) {
            sf::InputSoundFile file;
            if (!file.openFromFile(def.file)) {
                std::cerr << "Audio: cannot decode music; skipped: " << def.file << std::endl;
                continue;
            }

            preparedMusic.emplace(id, PreparedMusic{def});
        }

    } catch (const std::exception& error) {
        cleanup();
        std::cerr << "Audio: cannot prepare manifest: " << error.what() << std::endl;
        return false;
    } catch (...) {
        cleanup();
        std::cerr << "Audio: cannot prepare manifest: unknown error" << std::endl;
        return false;
    }

    sounds.swap(preparedSounds);
    music.swap(preparedMusic);
    cleanup();

    loaded = true;
    return true;
}


void SoundEngine::setListenerPosition(sf::Vector2f position) {
    if (finite(position)) {
        sf::Listener::setPosition({position.x / WorldUnitsPerMeter, position.y / WorldUnitsPerMeter, 0.f});
    }
}


void SoundEngine::update(float unscaledDeltaSeconds) {
    const float dt = duration(unscaledDeltaSeconds);
    cleanSounds();

    // Process queued commands
    while (!requests.empty()) {
        const Command command = requests.front();
        requests.pop();

        try {
            execute(command);
        } catch (const std::exception& error) {
            std::cerr << "Audio: " << error.what() << std::endl;
        }
    }

    // Refresh every active sound’s volume
    for (auto& instance : soundInstances) {
        instance.sound->setVolume(soundVolume(instance));
    }

    // Advance each music fade
    for (auto& instance : musicInstances) {
        instance.elapsed = std::min(instance.elapsed + dt, instance.fadeSeconds);

        const float t = instance.fadeSeconds > 0.f
            ? instance.elapsed / instance.fadeSeconds
            : 1.f;

        instance.gain = instance.startGain + (instance.targetGain - instance.startGain) * t;
        instance.stream->setVolume(100.f * masterGain() * channelGain(audioSettings.music) * instance.resource->definition.volume * instance.gain);
    }

    // Remove finished music instances
    for (auto it = musicInstances.begin(); it != musicInstances.end();) {
        if ((it->stopping && it->gain <= 0.f) || it->stream->getStatus() == sf::SoundSource::Status::Stopped) {
            delete it->stream;
            it = musicInstances.erase(it);
        } else {
            ++it;
        }
    }
}


AudioScopeId SoundEngine::createScope() {
    const AudioScopeId scope{nextScope++};
    scopes.insert(scope.value);
    return scope;
}


AudioScopeId SoundEngine::applicationScope() {
    return {1};
}


void SoundEngine::endScope(AudioScopeId scope) {
    if (scope.value == applicationScope().value || !scopes.erase(scope.value)) {
        return;
    }

    removeSounds([&](const SoundInstance& instance) -> bool {
        return instance.scope.value == scope.value;
    });

    for (auto& instance : musicInstances) {
        if (instance.scope.value == scope.value) {
            instance.stopping = true;
            instance.fadeTo(0.f, 0.15f);
        }
    }
}


SoundHandle SoundEngine::playSound(std::string_view id, AudioScopeId scope, const SoundPlaybackOptions& options) {
    if (!alive(scope)) {
        return {};
    }

    const auto resource = sounds.find(std::string(id));
    if (resource == sounds.end()) {
        if (missingSoundIds.emplace(id).second) {
            std::cerr << "Sound ID '" << id << "' has no loaded definition" << std::endl;
        }
        return {};
    }

    if (!std::isfinite(options.gain) || (options.position && !finite(*options.position))) {
        return {};
    }

    Command command;
    command.scope = resource->second.definition.lifetime == AudioLifetime::Application
        ? applicationScope()
        : scope;
    command.handle = {nextHandle++};
    command.soundId = id;
    command.options = options;
    command.options.gain = std::clamp(options.gain, 0.f, 1.f);
    return push(command)
        ? command.handle
        : SoundHandle{};
}


void SoundEngine::stopSound(SoundHandle handle) {
    Command command;
    command.type = CommandType::Stop;
    command.scope = applicationScope();
    command.handle = handle;
    push(command);
}


void SoundEngine::setSoundPosition(SoundHandle handle, sf::Vector2f position) {
    if (!finite(position)) {
        return;
    }

    Command command;
    command.type = CommandType::Position;
    command.scope = applicationScope();
    command.handle = handle;
    command.options.position = position;
    push(command);
}


bool SoundEngine::isPlaying(SoundHandle handle) const {
    return std::any_of(soundInstances.begin(), soundInstances.end(), [&](const SoundInstance& instance) -> bool {
        return instance.handle.value == handle.value && instance.sound->getStatus() != sf::SoundSource::Status::Stopped;
    });
}


void SoundEngine::playMusic(std::string_view id, AudioScopeId scope, float crossfadeSeconds) {
    if (!alive(scope)) {
        return;
    }

    const auto resource = music.find(std::string(id));
    if (resource == music.end()) {
        if (missingMusicIds.emplace(id).second) {
            std::cerr << "Music ID '" << id << "' has no loaded definition" << std::endl;
        }
        return;
    }

    Command command;
    command.type = CommandType::Music;
    command.scope = resource->second.definition.lifetime == AudioLifetime::Application
        ? applicationScope()
        : scope;
    command.musicId = id;
    command.value = duration(crossfadeSeconds);
    push(command);
}


void SoundEngine::stopMusic(float fadeSeconds) {
    Command command;
    command.type = CommandType::StopMusic;
    command.scope = applicationScope();
    command.value = duration(fadeSeconds);
    push(command);
}


std::optional<std::string> SoundEngine::currentMusic() const {
    for (auto it = musicInstances.rbegin(); it != musicInstances.rend(); ++it) {
        if (!it->stopping) {
            return it->id;
        }
    }
    return {};
}


std::size_t SoundEngine::activeSoundCount() const {
    return soundInstances.size();
}


std::size_t SoundEngine::activeMusicCount() const {
    return musicInstances.size();
}


void SoundEngine::stopAll() {
    requests = {};

    for (auto& instance : soundInstances) {
        delete instance.sound;
    }
    soundInstances.clear();
    for (auto& instance : musicInstances) {
        delete instance.stream;
    }
    musicInstances.clear();
}


void SoundEngine::MusicInstance::fadeTo(float target, float seconds) {
    startGain = gain;
    targetGain = target;
    elapsed = 0.f;
    fadeSeconds = seconds;

    if (seconds == 0.f) {
        gain = target;
    }
}


template<class Predicate>
void SoundEngine::removeSounds(Predicate predicate) {
    for (auto it = soundInstances.begin(); it != soundInstances.end();) {
        if (predicate(*it)) {
            delete it->sound;
            it = soundInstances.erase(it);
        } else {
            ++it;
        }
    }
}


bool SoundEngine::alive(AudioScopeId scope) const {
    return scopes.count(scope.value) != 0;
}


bool SoundEngine::push(Command command) {
    if (requests.size() >= MaxRequests) {
        std::cerr << "Audio request limit reached\n";
        return false;
    }
    requests.push(std::move(command));
    return true;
}


float SoundEngine::masterGain() const {
    return std::min(audioSettings.masterVolume, 100u) / 100.f;
}


float SoundEngine::soundVolume(const SoundInstance& instance) const {
    const auto& definition = instance.resource->definition;
    const auto& channel = definition.category == AudioCategory::UI
        ? audioSettings.uiSoundEffects
        : audioSettings.soundEffects;
    return 100.f * masterGain() * channelGain(channel) * definition.volume * instance.gain;
}


void SoundEngine::cleanSounds() {
    removeSounds([](const SoundInstance& instance) -> bool {
        return instance.sound->getStatus() == sf::SoundSource::Status::Stopped;
    });
}


void SoundEngine::startSound(const Command& command) {
    const auto& resource = sounds.at(command.soundId);
    const auto count = std::count_if(soundInstances.begin(), soundInstances.end(), [&](const SoundInstance& instance) -> bool {
        return instance.id == command.soundId;
    });

    if (soundInstances.size() >= MaxSoundInstances || count >= resource.definition.maxInstances) {
        return;
    }

    sf::Sound* pendingSound = nullptr;
    try {
        pendingSound = new sf::Sound(*resource.buffer);
        SoundInstance instance{command.handle, command.soundId, command.scope, &resource, command.options.gain, pendingSound};
        auto& sound = *instance.sound;
        sound.setLooping(command.options.looping.value_or(resource.definition.loop));
        const bool spatial = resource.definition.spatial && command.options.position.has_value();
        sound.setSpatializationEnabled(spatial);
        sound.setRelativeToListener(!spatial);
        sound.setMinDistance(resource.definition.minDistance / WorldUnitsPerMeter);
        sound.setAttenuation(resource.definition.attenuation);
        if (spatial) {
            const auto position = *command.options.position;
            sound.setPosition({position.x / WorldUnitsPerMeter, position.y / WorldUnitsPerMeter, 0.f});
        }
        sound.setVolume(soundVolume(instance));
        sound.play();
        soundInstances.push_back(instance);
        pendingSound = nullptr;

    } catch (const std::exception& error) {
        delete pendingSound;
        std::cerr << "Audio: cannot start sound '" << command.soundId << "': " << error.what() << std::endl;
    } catch (...) {
        delete pendingSound;
        std::cerr << "Audio: cannot start sound '" << command.soundId << "': unknown error" << std::endl;
    }
}


void SoundEngine::startMusic(const Command& command) {
    auto existing = std::find_if(musicInstances.begin(), musicInstances.end(), [&](const MusicInstance& instance) -> bool {
        return instance.id == command.musicId;
    });

    if (existing != musicInstances.end() && existing->stream->getStatus() != sf::SoundSource::Status::Stopped) {
        existing->scope = command.scope;

        if (existing->stopping) {
            existing->fadeTo(1.f, command.value);
        }

        existing->stopping = false;
        for (auto& instance : musicInstances) {
            if (&instance != &*existing) {
                instance.stopping = true;
                instance.fadeTo(0.f, command.value);
            }
        }
        return;
    }

    const PreparedMusic& resource = music.at(command.musicId);
    sf::Music* stream = nullptr;
    try {
        stream = new sf::Music;
        if (!stream->openFromFile(resource.definition.file)) {
            delete stream;
            stream = nullptr;
            std::cerr << "Audio: cannot open music: " << resource.definition.file << std::endl;
            return;
        }

        stream->setLooping(resource.definition.loop);
        stream->setSpatializationEnabled(false);
        stream->setRelativeToListener(true);
        stream->setVolume(0.f);
        stream->play();

        MusicInstance incoming{command.musicId, command.scope, &resource, stream};
        incoming.fadeTo(1.f, command.value);
        musicInstances.push_back(incoming);
        stream = nullptr;

    } catch (const std::exception& error) {
        delete stream;
        std::cerr << "Audio: cannot start music '" << command.musicId << "': " << error.what() << std::endl;
        return;
    } catch (...) {
        delete stream;
        std::cerr << "Audio: cannot start music '" << command.musicId << "': unknown error" << std::endl;
        return;
    }

    for (auto& instance : musicInstances) {
        if (&instance != &musicInstances.back()) {
            instance.stopping = true;
            instance.fadeTo(0.f, command.value);
        }
    }
}


void SoundEngine::execute(const Command& command) {
    if (!alive(command.scope)) {
        return;
    }

    switch (command.type) {
        case CommandType::Sound: {
            startSound(command);
            break;
        }
        case CommandType::Music: {
            startMusic(command);
            break;
        }
        case CommandType::Stop: {
            removeSounds([&](const SoundInstance& instance) -> bool {
                return instance.handle.value == command.handle.value;
            });
            break;
        }
        case CommandType::Position: {
            for (const auto& instance : soundInstances) {
                if (instance.handle.value == command.handle.value && instance.resource->definition.spatial) {
                    const auto position = *command.options.position;
                    instance.sound->setSpatializationEnabled(true);
                    instance.sound->setRelativeToListener(false);
                    instance.sound->setPosition({position.x / WorldUnitsPerMeter, position.y / WorldUnitsPerMeter, 0.f});
                }
            }
            break;
        }
        case CommandType::StopMusic: {
            for (auto& instance : musicInstances) {
                instance.stopping = true;
                instance.fadeTo(0.f, command.value);
            }
            break;
        }
    }
}
