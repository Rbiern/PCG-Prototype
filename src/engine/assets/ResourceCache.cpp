#include "ResourceCache.hpp"

#include "engine/assets/ResourceLoader.hpp"
#include <stdexcept>


template<typename T>
ResourceCache<T>::ResourceCache() = default;


template<typename T>
ResourceCache<T>::~ResourceCache() {
    for (const auto& entry : resources) {
        delete entry.second;
    }
}


template<typename T>
const T& ResourceCache<T>::get(const std::filesystem::path& filePath) {
    if (!filePath.is_absolute()) {
        throw std::invalid_argument("Resource cache requires an absolute resolved path");
    }

    // Look for a previously loaded resource
    const std::string key = filePath.generic_string();
    const auto found = resources.find(key);
    if (found != resources.end()) {
        return *found->second;
    }

    // Allocate a new resource
    T* resource = new T();
    try {
        if (!ResourceLoader<T>::load(*resource, filePath)) {
            throw std::runtime_error("Failed to load resource: " + filePath.string());
        }

        const auto inserted = resources.emplace(key, resource);
        if (!inserted.second) {
            delete resource;
        }
        return *inserted.first->second;
    } catch (...) {
        delete resource;
        throw;
    }
}


template class ResourceCache<sf::Texture>;
template class ResourceCache<sf::Font>;
