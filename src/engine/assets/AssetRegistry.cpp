#include "AssetRegistry.hpp"

#include <fstream>
#include <stdexcept>
#include <unordered_set>
#include <vector>


AssetRegistry::AssetRegistry(const std::filesystem::path& manifestPath) {
    try {
        const std::filesystem::path manifest = std::filesystem::canonical(manifestPath);
        std::ifstream stream(manifest);
        if (!stream) {
            throw std::runtime_error("Cannot open manifest");
        }

        // Reject duplicate keys before the parser overwrites an earlier value.
        std::vector<std::unordered_set<std::string>> objectKeys;
        auto callback = [&objectKeys](int, nlohmann::json::parse_event_t event, nlohmann::json& value) -> bool {
            if (event == nlohmann::json::parse_event_t::object_start) {
                objectKeys.emplace_back();
            } else if (event == nlohmann::json::parse_event_t::key) {
                const std::string key = value.get<std::string>();
                if (!objectKeys.back().insert(key).second) {
                    throw std::runtime_error("Duplicate JSON key: '" + key + "'");
                }
            } else if (event == nlohmann::json::parse_event_t::object_end) {
                objectKeys.pop_back();
            }
            return true;
        };

        const auto root = nlohmann::json::parse(stream, callback);
        if (!root.is_object()) {
            throw std::runtime_error("Manifest must be an object");
        }

        for (const auto& item : root.items()) {
            if (item.key() != "textures" && item.key() != "fonts") {
                throw std::runtime_error("Unknown manifest field: '" + item.key() + "'");
            }
        }

        textures = readSection(root, "textures", manifest.parent_path());
        fonts = readSection(root, "fonts", manifest.parent_path());
    } catch (const std::exception& error) {
        throw std::runtime_error("Asset manifest '" + manifestPath.string() + "': " + error.what());
    }
}


const std::filesystem::path& AssetRegistry::texturePath(std::string_view id) const {
    const auto it = textures.find(std::string(id));
    if (it == textures.end()) {
        throw std::runtime_error("Unknown texture ID: '" + std::string(id) + "'");
    }
    return it->second;
}


const std::filesystem::path& AssetRegistry::fontPath(std::string_view id) const {
    const auto it = fonts.find(std::string(id));
    if (it == fonts.end()) {
        throw std::runtime_error("Unknown font ID: '" + std::string(id) + "'");
    }
    return it->second;
}


AssetRegistry::PathMap AssetRegistry::readSection(const nlohmann::json& root, const char* name, const std::filesystem::path& directory) {
    const auto& section = root.at(name);
    if (!section.is_object()) {
        throw std::runtime_error(std::string(name) + " must be an object of ID-to-path entries");
    }

    PathMap paths;
    paths.reserve(section.size());
    for (const auto& item : section.items()) {
        const std::string context = std::string(name) + " asset '" + item.key() + "'";

        if (item.key().find_first_not_of(" \t\r\n") == std::string::npos || item.key().find('\0') != std::string::npos) {
            throw std::runtime_error("Invalid ID in " + std::string(name));
        }

        if (!item.value().is_string()) {
            throw std::runtime_error(context + " path must be a string");
        }

        const auto filename = item.value().get<std::string>();
        const std::filesystem::path relative(filename);

        if (filename.empty() || filename.find('\0') != std::string::npos || relative.has_root_path()) {
            throw std::runtime_error(context + " path must be nonempty and relative to the manifest");
        }

        std::error_code error;
        const auto resolved = std::filesystem::canonical(directory / relative, error);

        if (error) {
            throw std::runtime_error(context + " cannot resolve '" + (directory / relative).string() + "': " + error.message());
        }

        if (!std::filesystem::is_regular_file(resolved, error) || error) {
            throw std::runtime_error(context + " is not a readable regular-file path: " + resolved.string());
        }
        paths.emplace(item.key(), resolved);
    }
    return paths;
}
