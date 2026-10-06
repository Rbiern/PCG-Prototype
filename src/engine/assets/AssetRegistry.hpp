#pragma once

#include "infrastructure/json.hpp"
#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>


class AssetRegistry {
public:
    explicit AssetRegistry(const std::filesystem::path& manifestPath);

    [[nodiscard]] const std::filesystem::path& texturePath(std::string_view id) const;
    [[nodiscard]] const std::filesystem::path& fontPath(std::string_view id) const;

private:
    using PathMap = std::unordered_map<std::string, std::filesystem::path>;

    [[nodiscard]] static PathMap readSection(const nlohmann::json& root, const char* name, const std::filesystem::path& directory);

    PathMap textures;
    PathMap fonts;
};
