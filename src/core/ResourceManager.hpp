#pragma once
#include <iostream>
#include <memory>
#include <mutex>
#include <map>
#include <string>
#include <stdexcept>  // for std::runtime_error
#include <fstream>  // reading JSON file
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../json.hpp"


class ResourceManager {
public:
    ResourceManager(const ResourceManager&) = delete;           // Delete copy constructor
    ResourceManager& operator=(const ResourceManager&) = delete;// Delete assignment operator

    static ResourceManager& getInstance();  // Access the singleton instance
    static void destroyInstance();          // Destroy the singleton instance

    // Resource loading methods
    bool loadJsonConfig(const std::string& filename);
    sf::Font& getFont();
    sf::Texture& getTexture(const std::string& filePath);

    // Window variables
    unsigned int resolution[2];
    bool isFullscreen;
    bool verticalSync;
    bool antiAliasingEnabled;
    unsigned int antiAliasingLevel;
    unsigned int frameRate;

    // Difficulty and Level Generation Selections
    std::map<std::string, bool> generators;
    std::map<std::string, bool> inferenceEngine;

private:
    // Private constructor/destructor for singleton
    ResourceManager() = default;
    ~ResourceManager() = default;

    // Allow unique_ptr to call the private destructor.
    friend struct std::default_delete<ResourceManager>;

    static std::unique_ptr<ResourceManager> instance;
    static std::mutex mtx;

    // Resources
    sf::Font font;
    bool fontLoaded = false;
    std::unordered_map<std::string, sf::Texture> textures;
};
