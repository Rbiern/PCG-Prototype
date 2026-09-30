#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include "infrastructure/json.hpp"


struct VideoSettings {
    std::uint32_t windowWidth = 1280u;
    std::uint32_t windowHeight = 720u;
    std::uint32_t frameRate = 60u;
    std::uint32_t antiAliasingLevel = 0u;
    bool fullscreen = false;
    bool verticalSync = false;
};


struct AudioChannelSettings {
    std::uint32_t level = 100u;
    bool enabled = true;
};


struct AudioSettings {
    std::uint32_t masterVolume = 100u;
    AudioChannelSettings music{100u, true};
    AudioChannelSettings soundEffects{100u, true};
    AudioChannelSettings uiSoundEffects{100u, true};
};


struct GameplaySettings {
};


struct ApplicationSettings {
    std::filesystem::path path;
    VideoSettings video;
    AudioSettings audio;
    GameplaySettings gameplay;
    bool dirty = false;

    explicit ApplicationSettings(std::filesystem::path filePath);
    ~ApplicationSettings() = default;
    ApplicationSettings(const ApplicationSettings&) = delete;
    ApplicationSettings& operator=(const ApplicationSettings&) = delete;;

    bool load();
    [[nodiscard]] bool saveToDisk();

private:
    static bool readSettings(const nlohmann::json& root, VideoSettings& video, AudioSettings& audio);
    static bool readFileOneShot(const std::filesystem::path& inputPath, std::string& outPayload);
    [[nodiscard]] bool writeFileAtomically(const std::string& serialized) const;
};
