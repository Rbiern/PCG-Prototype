#pragma once

#include "AssetRegistry.hpp"
#include "ResourceCache.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <filesystem>
#include <string_view>


class ResourceManager {
public:
    explicit ResourceManager(const std::filesystem::path& manifestPath);
    ~ResourceManager() = default;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    [[nodiscard]] const sf::Texture& getTexture(std::string_view id);
    [[nodiscard]] const sf::Font& getFont(std::string_view id = "main");

private:
    AssetRegistry registry;
    ResourceCache<sf::Texture> textureCache;
    ResourceCache<sf::Font> fontCache;
};
