#include <core/ApplicationSettings.hpp>

#include <iostream>
#include <utility>
#include <algorithm>
#include "infrastructure/json.hpp"
#include "engine/io/FileIO.hpp"
#include "engine/io/JsonCallbacks.hpp"
#include "engine/io/JsonFields.hpp"


ApplicationSettings::ApplicationSettings(std::filesystem::path filePath)
    : path(std::move(filePath)) {
}


bool ApplicationSettings::load() {
    std::string payload;
    std::string error;

    if (!FileIO::readFileOneShot(path, payload, FileIO::OneMebibyte, error)) {
        std::cerr << "Failed to read config file: " << error << std::endl;
        dirty = true;
        return false;
    }

    try {
        const nlohmann::json jsonFile = nlohmann::json::parse(payload, JsonCallbacks::makeValidationCallback());
        if (!jsonFile.is_object()) {
            std::cerr << "Config root must be a JSON object, reverting to defaults: " << path << std::endl;
            dirty = true;
            return false;
        }

        VideoSettings nextVideo{};
        AudioSettings nextAudio{};
        GameplaySettings nextGameplay{};

        // Valid config values override defaults; missing or wrong-type values keep defaults.
        // Out-of-range values are clamped, and unsupported anti-aliasing levels are adjusted.
        const bool repaired = readSettings(jsonFile, nextVideo, nextAudio, nextGameplay);

        video = nextVideo;
        audio = nextAudio;
        gameplay = nextGameplay;
        dirty = repaired;
        return true;

    } catch (const nlohmann::json::exception& e) {
        std::cerr << "Invalid JSON in config file '" << path << "': " << e.what() << std::endl;
        dirty = true;
        return false;

    } catch (const std::runtime_error& e) {
        std::cerr << "Config validation failed for '" << path << "': " << e.what() << std::endl;
        dirty = true;
        return false;
    }
}


bool ApplicationSettings::saveToDisk() {
    try {
        const nlohmann::json jsonFile = {
            {"video", {
                {"fullscreen", video.fullscreen},
                {"verticalSync", video.verticalSync},
                {"frameRate", video.frameRate},
                {"antiAliasingLevel", video.antiAliasingLevel},
                {"resolution", {
                    {"width", video.windowWidth},
                    {"height", video.windowHeight}
                }}
            }},
            {"audio", {
                {"masterVolume", audio.masterVolume},
                {"music", {
                    {"enabled", audio.music.enabled},
                    {"level", audio.music.level}
                }},
                {"soundEffects", {
                    {"enabled", audio.soundEffects.enabled},
                    {"level", audio.soundEffects.level}
                }},
                {"uiSoundEffects", {
                    {"enabled", audio.uiSoundEffects.enabled},
                    {"level", audio.uiSoundEffects.level}
                }}
            }},
            {"gameplay", {
                {"showFrameRate", gameplay.showFrameRate}
            }}
        };

        std::string error;
        const bool saved = FileIO::writeFileAtomically(path, jsonFile.dump(4), error);

        if (!saved) {
            std::cerr << "Failed to save config: " << error << std::endl;
        }
        dirty = !saved;
        return saved;

    } catch (const std::exception& e) {
        dirty = true;
        std::cerr << "Failed to save config: " << e.what() << std::endl;
        return false;
    }
}


bool ApplicationSettings::readSettings(const nlohmann::json& root, VideoSettings& video, AudioSettings& audio, GameplaySettings& gameplay) {
    bool repaired = false;

    if (const auto* node = JsonFields::readObject(root, "video").valueOr(nullptr, &repaired)) {
        video.fullscreen = JsonFields::readBoolean(*node, "fullscreen").valueOr(video.fullscreen, &repaired);
        video.verticalSync = JsonFields::readBoolean(*node, "verticalSync").valueOr(video.verticalSync, &repaired);
        video.frameRate = JsonFields::readUInt32(*node, "frameRate", 30u, 360u).valueOr(video.frameRate, &repaired);

        constexpr std::array<std::uint32_t, 5> supportedAA{0u, 2u, 4u, 8u, 16u};

        uint32_t requestedAA = JsonFields::readUInt32(*node, "antiAliasingLevel", 0u, 16u).valueOr(video.antiAliasingLevel, &repaired);
        uint32_t closest = supportedAA.front();
        uint32_t closestDistance = requestedAA;

        for (const auto level : supportedAA) {
            const auto distance = level > requestedAA
                ? level - requestedAA
                : requestedAA - level;

            if (distance < closestDistance) {
                closest = level;
                closestDistance = distance;
            }
        }

        video.antiAliasingLevel = closest;

        if (closest != requestedAA) {
            repaired = true;
        }

        if (const auto* resolution = JsonFields::readObject(*node, "resolution").valueOr(nullptr, &repaired)) {
            video.windowWidth = JsonFields::readUInt32(*resolution, "width", 960u, 7680u).valueOr(video.windowWidth, &repaired);
            video.windowHeight = JsonFields::readUInt32(*resolution, "height", 540u, 4320u).valueOr(video.windowHeight, &repaired);
        }
    }

    if (const auto* node = JsonFields::readObject(root, "audio").valueOr(nullptr, &repaired)) {
        audio.masterVolume = JsonFields::readUInt32(*node, "masterVolume", 0u, 100u).valueOr(audio.masterVolume, &repaired);

        const std::pair<const char*, AudioChannelSettings*> channels[]{
            {"music", &audio.music},
            {"soundEffects", &audio.soundEffects},
            {"uiSoundEffects", &audio.uiSoundEffects}
        };

        for (const auto& [name, target] : channels) {
            if (const auto* channel = JsonFields::readObject(*node, name).valueOr(nullptr, &repaired)) {
                target->enabled = JsonFields::readBoolean(*channel, "enabled").valueOr(target->enabled, &repaired);
                target->level = JsonFields::readUInt32(*channel, "level", 0u, 100u).valueOr(target->level, &repaired);
            }
        }
    }

    if (const auto* node = JsonFields::readObject(root, "gameplay").valueOr(nullptr, &repaired)) {
        gameplay.showFrameRate = JsonFields::readBoolean(*node, "showFrameRate").valueOr(gameplay.showFrameRate, &repaired);
    }

    return repaired;
}
