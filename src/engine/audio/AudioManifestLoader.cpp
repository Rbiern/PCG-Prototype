#include "AudioManifestLoader.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>
#include <initializer_list>
#include "infrastructure/json.hpp"
#include "engine/io/FileIO.hpp"
#include "engine/io/JsonCallbacks.hpp"
#include "engine/io/JsonFields.hpp"


AudioManifestLoader::AudioManifestLoader(std::filesystem::path path)
    : path(std::move(path)) {
}


const AudioCatalog& AudioManifestLoader::catalog() const noexcept {
    return audioCatalog;
}


bool AudioManifestLoader::load(std::string& error) {
    error.clear();
    std::string payload;

    if (!FileIO::readFileOneShot(path, payload, FileIO::OneMebibyte, error)) {
        error = path.u8string() + ": " + error;
        return false;
    }

    try {
        const auto root = nlohmann::json::parse(payload, JsonCallbacks::makeValidationCallback());
        if (!root.is_object()) {
            throw std::runtime_error("Manifest root must be an object");
        }

        constexpr std::string_view IdStart = "abcdefghijklmnopqrstuvwxyz0123456789";
        constexpr std::string_view IdCharacters = "abcdefghijklmnopqrstuvwxyz0123456789._-";

        AudioCatalog next;
        for (const char* sectionName : {"sounds", "music"}) {
            const auto* section = JsonFields::readObject(root, sectionName).valueOr(nullptr);
            if (!section) {
                continue;
            }

            const bool isSound = std::string_view(sectionName) == "sounds";

            for (const auto& entry : section->items()) {
                const auto& id = entry.key();
                if (id.empty() || IdStart.find(id.front()) == std::string_view::npos || id.find_first_not_of(IdCharacters) != std::string::npos) {
                    continue;
                }

                const auto& node = entry.value();
                if (!node.is_object()) {
                    continue;
                }

                const auto filename = JsonFields::readString(node, "file");
                if (!filename.value || filename.value->empty() || filename.value->find('\0') != std::string::npos) {
                    continue;
                }

                std::filesystem::path file;

                // File paths must be relative and resolve to a file.
                try {
                    const auto relative = std::filesystem::u8path(*filename.value);
                    if (relative.has_root_path()) {
                        continue;
                    }

                    std::error_code fileError;
                    file = std::filesystem::canonical(path.parent_path() / relative, fileError);

                    if (fileError || !std::filesystem::is_regular_file(file, fileError) || fileError) {
                        continue;
                    }

                } catch (const std::filesystem::filesystem_error&) {
                    continue;
                }

                const auto lifetimeName = JsonFields::readChoice(node, "lifetime", {"scope", "application"}).valueOr("scope");
                const auto lifetime = lifetimeName == "application"
                        ? AudioLifetime::Application
                        : AudioLifetime::Scope;

                if (isSound) {
                    SoundDefinition definition;

                    definition.file = std::move(file);
                    definition.lifetime = lifetime;
                    definition.volume = JsonFields::readFloat(node, "volume", 0.f, 1.f).valueOr(definition.volume);
                    definition.loop = JsonFields::readBoolean(node, "loop").valueOr(definition.loop);
                    definition.spatial = JsonFields::readBoolean(node, "spatial").valueOr(definition.spatial);
                    definition.minDistance = JsonFields::readFloat(node, "minDistance", 1.f, 1'000'000.f).valueOr(definition.minDistance);
                    definition.attenuation = JsonFields::readFloat(node, "attenuation", 0.f, 100.f).valueOr(definition.attenuation);
                    definition.maxInstances = JsonFields::readUInt32(node, "maxInstances", 1u, 64u).valueOr(definition.maxInstances);

                    const std::string category = JsonFields::readChoice(node, "category", {"sfx", "ui"}).valueOr("sfx");
                    definition.category = category == "ui"
                        ? AudioCategory::UI
                        : AudioCategory::SFX;

                    if (definition.category == AudioCategory::UI) {
                        definition.spatial = false;
                    }

                    next.sounds.emplace(id, std::move(definition));
                } else {
                    MusicDefinition definition;

                    definition.file = std::move(file);
                    definition.lifetime = lifetime;
                    definition.volume =JsonFields::readFloat(node, "volume", 0.f, 1.f).valueOr(definition.volume);
                    definition.loop = JsonFields::readBoolean(node, "loop").valueOr(definition.loop);

                    next.music.emplace(id, std::move(definition));
                }
            }
        }

        audioCatalog.sounds.swap(next.sounds);
        audioCatalog.music.swap(next.music);

        return true;

    } catch (const nlohmann::json::exception& exception) {
        error = path.u8string() + ": " + "Invalid JSON: " + exception.what();

    } catch (const std::runtime_error& exception) {
        error = path.u8string() + ": " + exception.what();
    }

    return false;
}
