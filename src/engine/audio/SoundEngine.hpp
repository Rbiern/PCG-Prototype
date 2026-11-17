#pragma once

#include "AudioDefinitions.hpp"
#include <filesystem>
#include <queue>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace sf {
class SoundBuffer;
class Sound;
class Music;
}

struct AudioSettings;

class SoundEngine {
public:
    explicit SoundEngine(const AudioSettings& audioSettings);
    ~SoundEngine();
    SoundEngine(const SoundEngine&) = delete;
    SoundEngine& operator=(const SoundEngine&) = delete;

    [[nodiscard]] bool loadManifest(const std::filesystem::path& manifestPath);
    void setListenerPosition(sf::Vector2f position);
    void update(float unscaledDeltaSeconds);

    [[nodiscard]] AudioScopeId createScope();
    [[nodiscard]] static AudioScopeId applicationScope();
    void endScope(AudioScopeId scope);

    SoundHandle playSound(std::string_view id, AudioScopeId scope, const SoundPlaybackOptions& options = {});
    void stopSound(SoundHandle handle);
    void setSoundPosition(SoundHandle handle, sf::Vector2f position);
    [[nodiscard]] bool isPlaying(SoundHandle handle) const;

    void playMusic(std::string_view id, AudioScopeId scope, float crossfadeSeconds = 0.5f);
    void stopMusic(float fadeSeconds = 0.5f);
    [[nodiscard]] std::optional<std::string> currentMusic() const;

    [[nodiscard]] std::size_t activeSoundCount() const;
    [[nodiscard]] std::size_t activeMusicCount() const;
    void stopAll();

private:
    struct PreparedSound {
        SoundDefinition definition;
        sf::SoundBuffer* buffer = nullptr;
    };

    struct PreparedMusic {
        MusicDefinition definition;
    };

    struct SoundInstance {
        SoundHandle handle;
        std::string id;
        AudioScopeId scope;
        const PreparedSound* resource;
        float gain = 1.f;
        sf::Sound* sound = nullptr;
    };

    struct MusicInstance {
        std::string id;
        AudioScopeId scope;
        const PreparedMusic* resource;
        sf::Music* stream = nullptr;
        float gain = 0.f;
        float startGain = 0.f;
        float targetGain = 1.f;
        float elapsed = 0.f;
        float fadeSeconds = 0.f;
        bool stopping = false;

        void fadeTo(float target, float seconds);
    };

    enum class CommandType {
        Sound, Stop, Position, Music, StopMusic
    };

    struct Command {
        CommandType type = CommandType::Sound;
        AudioScopeId scope;
        SoundHandle handle;
        std::string soundId;
        std::string musicId;
        SoundPlaybackOptions options;
        float value = 0.f;
    };

    template<class Predicate> void removeSounds(Predicate predicate);
    [[nodiscard]] bool alive(AudioScopeId scope) const;
    bool push(Command command);
    [[nodiscard]] float masterGain() const;
    [[nodiscard]] float soundVolume(const SoundInstance& instance) const;
    void cleanSounds();
    void startSound(const Command& command);
    void startMusic(const Command& command);
    void execute(const Command& command);

    const AudioSettings& audioSettings;
    std::unordered_map<std::string, PreparedSound> sounds;
    std::unordered_map<std::string, PreparedMusic> music;
    std::vector<SoundInstance> soundInstances;
    std::vector<MusicInstance> musicInstances;
    std::queue<Command> requests;
    std::unordered_set<std::uint64_t> scopes{1};
    std::uint64_t nextScope = 2;
    std::uint64_t nextHandle = 1;
    std::unordered_set<std::string> missingSoundIds;
    std::unordered_set<std::string> missingMusicIds;
    bool loaded = false;
};
