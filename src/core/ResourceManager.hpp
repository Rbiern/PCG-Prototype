#pragma once
#include <windows.h>
#include <shellapi.h>
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
    bool fullscreen;
    bool verticalSync;
    bool antiAliasing;
    bool music;
    bool soundEffect;
    unsigned int musicLevel;
    unsigned int soundLevel;
    unsigned int antiAliasingLevel;
    unsigned int frameRate;
    sf::Vector2u windowSize;
    bool updateRequired = false;
};

// Tower variables
struct TowerData {
    std::string name;
    std::string towerImagePath_a;
    std::string towerImagePath_b;
    std::string towerClass;
    std::string classIconImagePath;
    std::string position;
    std::string role;
    int HP;
    int ATK;
    int RES;
    int cost;
    int blockCount;
    float attackInterval;
    std::vector<sf::Vector2i> tileRange;
};


struct decks {
    std::array<TowerData*, 8> deck1;
    std::array<TowerData*, 8> deck2;
    std::array<TowerData*, 8> deck3;
    std::array<TowerData*, 8> deck4;
};

// Level grid variables
struct gridConfig {
    int rows = 8; //8 , 12, 16
    int cols = 12;//12 , 18, 24
    float gridWidth = 1280 * 0.85f;
    float gridHeight = 720;
    sf::Vector2i gridSize = {rows, cols};
    float squareWidth = gridWidth / cols;
    float squareHeight = gridHeight / rows;
};


//struct levelConfig {
//    float obstacles;
//    float paths;
//    float mapSize;
//    float spawnPoints;
//    float defendPoints;
//    float pathLoops;
//};
//
//
//struct playerPerformance {
//    int deployedTowersCount;
//    int health;
//    int resources;
//    float killTime;
//    float waveTime;
//};


class ResourceManager {
public:
    ResourceManager(const ResourceManager&) = delete;           // Delete copy constructor
    ResourceManager& operator=(const ResourceManager&) = delete;// Delete assignment operator
    static ResourceManager& getInstance();                      // Access the singleton instance
    static void destroyInstance();                              // Destroy the singleton instance

    // Utilizes      methods and getters
    sf::Font& getFont();
    sf::Texture& getTexture(const std::string& filePath);
    TowerData getTowerData(const std::string& name) const;
    bool updateWindowConfig(const std::string& filePath);
    windowConfig& getWindowData();
//    const windowConfig& getWindowData() const;
    const gridConfig& getGridData() const;
    sf::Vector2f getScaling();

    // Resource loading
    bool loadJsonConfig(const std::string& filename);
    bool loadTowerData(const std::string& jsonPath);

private:
    // Private constructor/destructor for singleton
    ResourceManager() = default;
    ~ResourceManager() = default;

    // Allow unique_ptr to call the private destructor.
    friend struct std::default_delete<ResourceManager>;

    static std::unique_ptr<ResourceManager> instance;
    static std::mutex mtx;

    bool IsTaskbarAutoHide();
    bool GetTaskbarData(APPBARDATA& abd);

    // Resources
    sf::Font font;
    bool fontLoaded = false;
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, TowerData> towerDataMap;


    // Structures
    windowConfig windowInfo;
    gridConfig gridCfg;
    decks towerDeck;
};
