#include <core/ApplicationSettings.hpp>

#include <fstream>
#include <iostream>
#include <algorithm>
#include <atomic>
#include <Windows.h>
#include <infrastructure/json.hpp>


ApplicationSettings::ApplicationSettings(std::filesystem::path filePath)
    : path(std::move(filePath)) {
}


bool ApplicationSettings::load() {
    std::string payload;
    if (!readFileOneShot(path, payload)) {
        std::cerr << "Config file not found or unreadable, writing defaults: " << path << std::endl;
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

        // Valid config values override defaults; missing or wrong-type values keep defaults.
        // Out-of-range values are clamped, and unsupported anti-aliasing levels are adjusted.
        const bool repaired = readSettings(jsonFile, nextVideo, nextAudio);
        video = nextVideo;
        audio = nextAudio;
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
            {"gameplay", nlohmann::json::object()}
        };

        const bool saved = writeFileAtomically(jsonFile.dump(4));
        dirty = !saved;
        return saved;

    } catch (const std::exception& e) {
        dirty = true;
        std::cerr << "Failed to save config: " << e.what() << std::endl;
        return false;
    }
}


bool ApplicationSettings::readSettings(const nlohmann::json& root, VideoSettings& video, AudioSettings& audio) {
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

    return repaired;
}


bool ApplicationSettings::readFileOneShot(const std::filesystem::path& inputPath, std::string& outPayload) {
    std::ifstream file(inputPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return false;
    }

    const std::streamsize endPos = file.tellg();
    if (endPos < 0) {
        return false;
    }

    outPayload.resize(static_cast<std::size_t>(endPos));
    file.seekg(0, std::ios::beg);

    if (!outPayload.empty()) {
        file.read(outPayload.data(), endPos);
        if (!file.good()) {
            return false;
        }
    }

    return true;
}


bool ApplicationSettings::writeFileAtomically(const std::string& serialized) const {
    std::error_code ec;
    const std::filesystem::path destination = std::filesystem::absolute(path, ec);

    if (ec) {
        std::cerr << "Invalid config path: " << ec.message() << std::endl;
        return false;
    }

    std::filesystem::create_directories(destination.parent_path(), ec);
    if (ec) {
        std::cerr << "Failed to create config directory: " << ec.message() << std::endl;
        return false;
    }

    // CREATE_NEW prevents this save from overwriting another save's temp file.
    static std::atomic<std::uint64_t> nextId{0};
    std::filesystem::path tempPath;
    HANDLE file = INVALID_HANDLE_VALUE;

    for (int attempt = 0; attempt < 32; ++attempt) {
        tempPath = destination;
        tempPath += L".tmp." + std::to_wstring(GetCurrentProcessId()) + L"." + std::to_wstring(nextId.fetch_add(1));

        file = CreateFileW(
            tempPath.c_str(),
            GENERIC_WRITE,
            0,
            nullptr,
            CREATE_NEW,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (file != INVALID_HANDLE_VALUE) {
            break;
        }

        const DWORD error = GetLastError();
        if (error != ERROR_FILE_EXISTS &&
            error != ERROR_ALREADY_EXISTS) {
            std::cerr << "Failed to create config temp file: "
                      << std::error_code(static_cast<int>(error), std::system_category()).message()
                      << std::endl;
            return false;
        }
    }

    if (file == INVALID_HANDLE_VALUE) {
        std::cerr << "Could not find a unique config temp filename" << std::endl;
        return false;
    }

    DWORD writeError = ERROR_SUCCESS;
    std::size_t offset = 0;

    while (offset < serialized.size()) {
        const std::size_t remaining = serialized.size() - offset;
        const DWORD requested = std::min<std::size_t>(remaining, std::numeric_limits<DWORD>::max());

        DWORD written = 0;
        if (!WriteFile(file, serialized.data() + offset, requested, &written, nullptr) || written == 0) {
            writeError = GetLastError();
            if (writeError == ERROR_SUCCESS) {
                writeError = ERROR_WRITE_FAULT;
            }
            break;
        }

        offset += written;
    }

    if (writeError == ERROR_SUCCESS && !FlushFileBuffers(file)) {
        writeError = GetLastError();
    }

    if (!CloseHandle(file) && writeError == ERROR_SUCCESS) {
        writeError = GetLastError();
    }

    if (writeError != ERROR_SUCCESS) {
        std::cerr << "Failed writing config temp file: "
                  << std::error_code(static_cast<int>(writeError), std::system_category()).message()
                  << std::endl;
        std::filesystem::remove(tempPath, ec);
        return false;
    }

    // The temp file is in the destination directory, so this stays
    // on the same volume. The old config is never renamed away first.
    if (!MoveFileExW(tempPath.c_str(), destination.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        const DWORD error = GetLastError();
        std::cerr << "Failed replacing config: "
                  << std::error_code(static_cast<int>(error), std::system_category()).message()
                  << std::endl;
        std::filesystem::remove(tempPath, ec);
        return false;
    }

    return true;
}
