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


bool ResourceManager::loadJsonConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    nlohmann::json j;
    file >> j;
//    std::cout << "Loaded JSON:\n" << j.dump(4) << std::endl;

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


sf::Font &ResourceManager::getFont() {
    if (!fontLoaded) {
        if (!font.openFromFile("../../font/Jersey15-Regular.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
            exit(-1);
        }
    }
    return font;
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
        data.imagePath = tower["imagePath"];
        data.cost = tower["cost"];
        data.towerClass = tower["class"];
        data.classImage = tower["classImage"];

        towerDataMap[data.name] = data;
    }
    return true;
}

TowerData ResourceManager::getTowerDataByName(const std::string& name) const {
    auto it = towerDataMap.find(name);
    if (it != towerDataMap.end()) {
        return it->second;
    }

    std::cerr << "Tower not found: " << name << std::endl;
    return TowerData{}; // return empty/default
}
