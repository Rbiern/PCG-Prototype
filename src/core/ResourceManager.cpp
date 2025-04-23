#include "ResourceManager.hpp"

// Initialize static members
std::unique_ptr<ResourceManager> ResourceManager::instance = nullptr;
std::mutex ResourceManager::mtx;


ResourceManager& ResourceManager::getInstance() {
    if (!instance) {
        std::lock_guard<std::mutex> lock(mtx);
        if (!instance) {
            instance.reset(new ResourceManager());
        }
    }
    return *instance;
}


void ResourceManager::destroyInstance() {
    std::lock_guard<std::mutex> lock(mtx);
    instance.reset();
}


sf::Font &ResourceManager::getFont() {
    if (!fontLoaded) {
        if (!font.openFromFile("../../font/Jersey15-Regular.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
            exit(-1);
        }
        fontLoaded = true;
    }
    return font;
}


bool ResourceManager::loadJsonConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    nlohmann::json j;
    file >> j;

    // Window fields
    windowInfo.resolution[0] = j["resolution"][0];
    windowInfo.resolution[1] = j["resolution"][1];
    windowInfo.isFullscreen = j["isFullscreen"];
    windowInfo.verticalSync = j["verticalSync"];
    windowInfo.frameRate = j["frameRate"];
    const auto& aa = j["antiAliasingLevel"];
    windowInfo.antiAliasingEnabled = aa["enabled"];
    windowInfo.antiAliasingLevel = aa["level"];

    // Load 'generators' map
    if (j.contains("generators") && j["generators"].is_object()) {
        for (auto& [key, value] : j["generators"].items()) {
            if (value.is_boolean()) {
                generators[key] = value.get<bool>();
            }
        }
    }

    return true;
}


sf::Texture& ResourceManager::getTexture(const std::string& filePath) {
    auto it = textures.find(filePath);
    if (it != textures.end()) {
        return it->second;
    }

    sf::Texture texture;
    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Failed to load texture: " << filePath << '\n';
    }
    textures[filePath] = std::move(texture);
    return textures[filePath];
}


bool ResourceManager::loadTowerData(const std::string& jsonPath) {
    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open tower JSON file: " << jsonPath << std::endl;
        return false;
    }

    nlohmann::json j;
    file >> j;

    for (const auto& tower : j["towers"]) {
        TowerData data;
        data.name = tower["name"];
        data.imagePath_a = tower["imagePath_a"];
        data.imagePath_b = tower["imagePath_b"];
        data.towerClass = tower["class"];
        data.classImage = tower["classImage"];
        data.cost = tower["cost"];
        data.piercing = tower["piercing"];
        data.attackDamage = tower["attackDamage"];
        data.attackCoolDown = tower["attackCoolDown"];

        for (const auto& coord : tower["tileRange"]) {
            if (coord.size() == 2) {
                data.tileRange.emplace_back(coord[0], coord[1]);
            }
        }

        towerDataMap[data.name] = data;
    }

    return true;
}


TowerData ResourceManager::getTowerData(const std::string& name) const {
    auto it = towerDataMap.find(name);
    if (it != towerDataMap.end()) {
        return it->second;
    }
    std::cerr << "Tower not found: " << name << std::endl;
    return TowerData{};
}


const windowConfig& ResourceManager::getWindowData() const {
    return windowInfo;
}


const gridConfig& ResourceManager::getGridData() const {
    return gridCfg;
}
