#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>


namespace sf {
class Texture;
class Font;
}

template<typename T>
class ResourceCache {
public:
    ResourceCache();
    ~ResourceCache();

    ResourceCache(const ResourceCache&) = delete;
    ResourceCache& operator=(const ResourceCache&) = delete;
    ResourceCache(ResourceCache&&) = delete;
    ResourceCache& operator=(ResourceCache&&) = delete;

    [[nodiscard]] const T& get(const std::filesystem::path& filePath);

private:
    std::unordered_map<std::string, T*> resources;
};

extern template class ResourceCache<sf::Texture>;
extern template class ResourceCache<sf::Font>;
