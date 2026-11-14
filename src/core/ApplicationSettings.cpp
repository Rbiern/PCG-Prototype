#include <core/ApplicationSettings.hpp>

#include <iostream>
#include <utility>
#include <algorithm>
#include <infrastructure/json.hpp>
#include "engine/io/FileIO.hpp"


ApplicationSettings::ApplicationSettings(std::filesystem::path filePath)
    : path(std::move(filePath)) {
}


bool ApplicationSettings::load() {
    std::string payload;
    std::string error;

    if (!FileIO::readFileOneShot(path, payload, FileIO::OneMebibyte, error)) {
        std::cerr << "Failed to read config file: " << error << std::endl;
        return false;
    }

    try {
        const nlohmann::json jsonFile = nlohmann::json::parse(payload);
        if (!jsonFile.is_object()) {
            std::cerr << "Config root must be a JSON object, reverting to defaults: " << path << std::endl;
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
        std::cerr << "Failed to parse config file, reverting to defaults: " << e.what() << std::endl;
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

    const auto object = [&](const nlohmann::json& parent, const char* key) -> const nlohmann::json* {
        const auto it = parent.find(key);
        if (it != parent.end() && it->is_object()) {
            return &*it;
        }
        repaired = true;
        return nullptr;
    };

    const auto boolean = [&](const nlohmann::json& parent, const char* key, bool fallback) -> bool {
        const auto it = parent.find(key);
        if (it != parent.end() && it->is_boolean()) {
            return it->get<bool>();
        }
        repaired = true;
        return fallback;
    };

    const auto number = [&](const nlohmann::json& parent, const char* key, std::uint32_t fallback, std::uint32_t minimum, std::uint32_t maximum) -> std::uint32_t {
        const auto it = parent.find(key);
        if (it == parent.end() || !it->is_number_integer()) {
            repaired = true;
            return fallback;
        }

        if (it->is_number_unsigned()) {
            const auto value = it->get<std::uint64_t>();
            const auto bounded = std::clamp(value, static_cast<std::uint64_t>(minimum), static_cast<std::uint64_t>(maximum));

            if (bounded != value) {
                repaired = true;
            }
            return static_cast<std::uint32_t>(bounded);
        }

        const auto value = it->get<std::int64_t>();
        const auto bounded = std::clamp(value, static_cast<std::int64_t>(minimum), static_cast<std::int64_t>(maximum));

        if (bounded != value) {
            repaired = true;
        }
        return static_cast<std::uint32_t>(bounded);
    };

    const auto channel = [&](const nlohmann::json& parent, const char* key, AudioChannelSettings& target) -> void {
        if (const auto* node = object(parent, key)) {
            target.enabled = boolean(*node, "enabled", target.enabled);
            target.level = number(*node, "level", target.level, 0u, 100u);
        }
    };

    if (const auto* node = object(root, "video")) {
        video.fullscreen = boolean(*node, "fullscreen", video.fullscreen);
        video.verticalSync = boolean(*node, "verticalSync", video.verticalSync);
        video.frameRate = number(*node, "frameRate", video.frameRate, 30u, 360u);

        const auto requestedAA = number(*node, "antiAliasingLevel", video.antiAliasingLevel, 0u, 16u);
        constexpr std::array<std::uint32_t, 5> supportedAA{0u, 2u, 4u, 8u, 16u};

        const auto closest = std::min_element(supportedAA.begin(), supportedAA.end(), [requestedAA](std::uint32_t a, std::uint32_t b) -> bool {
            const auto distance = [requestedAA](std::uint32_t level) -> std::uint32_t {
                return level > requestedAA
                    ? level - requestedAA
                    : requestedAA - level;
            };
            return distance(a) < distance(b);
        });

        video.antiAliasingLevel = *closest;
        if (*closest != requestedAA) repaired = true;

        if (const auto* resolution = object(*node, "resolution")) {
            video.windowWidth = number(*resolution, "width", video.windowWidth, 960u, 7680u);
            video.windowHeight = number(*resolution, "height", video.windowHeight, 540u, 4320u);
        }
    }

    if (const auto* node = object(root, "audio")) {
        audio.masterVolume = number(*node, "masterVolume", audio.masterVolume, 0u, 100u);
        channel(*node, "music", audio.music);
        channel(*node, "soundEffects", audio.soundEffects);
        channel(*node, "uiSoundEffects", audio.uiSoundEffects);
    }

    if (const auto* node = object(root, "gameplay")) {
        gameplay.showFrameRate = boolean(*node, "showFrameRate", gameplay.showFrameRate);
    }

    return repaired;
}
