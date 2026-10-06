#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <filesystem>


template<typename T>
struct ResourceLoader;

template<>
struct ResourceLoader<sf::Texture> {
    static bool load(sf::Texture& texture, const std::filesystem::path& path);
};

template<>
struct ResourceLoader<sf::Font> {
    static bool load(sf::Font& font, const std::filesystem::path& path);
};
