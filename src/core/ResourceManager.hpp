#pragma once
#include <iostream>
#include <memory>
#include <mutex>
#include <map>
#include <string>
#include <stdexcept>
#include <fstream>
#include <SFML/Graphics.hpp>
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
    std::string imagePath_a;
    std::string imagePath_b;
    std::string towerClass;
    std::string classImage;
    int cost;
    bool piercing;
    int attackDamage;
    float attackCoolDown;
    std::vector<sf::Vector2i> tileRange;
};

// Level grid variables
struct gridConfig {
    int rows = 8; //8 10 12 12
    int cols = 12;//12 7 8  18
    float gridWidth = 1280 * 0.85f;
    float gridHeight = 720;
    sf::Vector2i gridSize = {rows, cols};
    float squareWidth = gridWidth / cols;
    float squareHeight = gridHeight / rows;
};


struct levelConfig {
    float obstacles;
    float paths;
    float mapSize;
    float spawnPoints;
    float defendPoints;
    float pathLoops;
};


struct playerPerformance {
    int deployedTowersCount;
    int health;
    int resources;
    float killTime;
    float waveTime;
};


class ResourceManager {
public:
    ResourceManager(const ResourceManager&) = delete;           // Delete copy constructor
    ResourceManager& operator=(const ResourceManager&) = delete;// Delete assignment operator
    static ResourceManager& getInstance();                      // Access the singleton instance
        static void destroyInstance();                          // Destroy the singleton instance

    // Resource loading methods
    sf::Font& getFont();
    bool loadJsonConfig(const std::string& filename);
    sf::Texture& getTexture(const std::string& filePath);
    bool loadTowerData(const std::string& jsonPath);
    TowerData getTowerData(const std::string& name) const;
    const windowConfig& getWindowData() const;
    const gridConfig& getGridData() const;

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
    // Difficulty and Level Generation Selections
    std::map<std::string, bool> generators;
    std::map<std::string, bool> inferenceEngine;
    // Structures
    windowConfig windowInfo;
    gridConfig gridCfg;
};
