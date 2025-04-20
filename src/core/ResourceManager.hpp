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

// Window variables
struct windowConfig {
    unsigned int resolution[2];
    bool isFullscreen;
    bool verticalSync;
    bool antiAliasingEnabled;
    unsigned int antiAliasingLevel;
    unsigned int frameRate;
};

// Tower variables
struct TowerData {
    std::string name;
    std::string imagePath;
    int cost;
    std::string towerClass;
    std::string classImage;
};

// Grid values
struct gridConfig {
    float gridWidth = 1280 * 0.85f;
    float gridHeight = 720;
    sf::Vector2i gridSize = {12, 18};
    float squareWidth = gridWidth / gridSize.y;
    float squareHeight = gridHeight / gridSize.x;
};


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

    // Difficulty and Level Generation Selections
    std::map<std::string, bool> generators;
    std::map<std::string, bool> inferenceEngine;

    bool loadTowerData(const std::string& jsonPath);
    TowerData getTowerDataByName(const std::string& name) const;
    const windowConfig& getWindowData() const {
        return windowInfo;
    }
    const gridConfig& getGridData() const {
        return gridCfg;
    }

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
    std::unordered_map<std::string, TowerData> towerDataMap;

    windowConfig windowInfo;
    gridConfig gridCfg;
};
