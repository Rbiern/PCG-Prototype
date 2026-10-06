#include "ResourceLoader.hpp"


bool ResourceLoader<sf::Texture>::load(sf::Texture& texture, const std::filesystem::path& path) {
    return texture.loadFromFile(path);
}


bool ResourceLoader<sf::Font>::load(sf::Font& font, const std::filesystem::path& path) {
    return font.openFromFile(path);
}
