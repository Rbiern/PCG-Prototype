#include "ResourceManager.hpp"

#include <stdexcept>
#include <string>


ResourceManager::ResourceManager(const std::filesystem::path& manifestPath)
    : registry(manifestPath) {
}


const sf::Texture& ResourceManager::getTexture(std::string_view id) {
    const std::filesystem::path& path = registry.texturePath(id);
    try {
        return textureCache.get(path);
    } catch (const std::exception& error) {
        throw std::runtime_error("Texture '" + std::string(id) + "': " + error.what());
    }
}


const sf::Font& ResourceManager::getFont(std::string_view id) {
    const std::filesystem::path& path = registry.fontPath(id);
    try {
        return fontCache.get(path);
    } catch (const std::exception& error) {
        throw std::runtime_error("Font '" + std::string(id) + "': " + error.what());
    }
}
