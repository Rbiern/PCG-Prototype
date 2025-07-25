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


sf::Font& ResourceManager::getFont() {
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
    windowInfo.windowSize.x = j["resolution"][0];
    windowInfo.windowSize.y = j["resolution"][1];
    windowInfo.fullscreen = j["fullscreen"];
    windowInfo.verticalSync = j["vertical sync"];
    windowInfo.frameRate = j["frame rate"];

    const auto& aa = j["anti-aliasing level"];
    windowInfo.antiAliasing = aa["enabled"];
    windowInfo.antiAliasingLevel = aa["level"];

    const auto& m = j["music"];
    windowInfo.music = m["enabled"];
    windowInfo.musicLevel = m["level"];

    const auto& s = j["sound effect"];
    windowInfo.soundEffect = s["enabled"];
    windowInfo.soundLevel = s["level"];

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

        data.name = tower["Name"];
        data.towerImagePath_a = tower["Image_Path_a"];
        data.towerImagePath_b = tower["Image_Path_b"];
        data.towerClass = tower["Class"];
        data.classIconImagePath = tower["Class icon"];
        data.position = tower["Position"];
        data.role = tower["Role"];
        data.HP = tower["HP"];
        data.ATK = tower["ATK"];
        data.RES = tower["RES"];
        data.cost = tower["Cost"];
        data.blockCount = tower["Block count"];
        data.attackInterval = tower["Attack interval"];

        for (const auto& coord : tower["Tile range"]) {
            if (coord.size() == 2) {
                data.tileRange.emplace_back(coord[0], coord[1]);
            }
        }

        towerDataMap[data.name] = data;
    }

    return true;
}

/// Returns true on success, false on error.
bool ResourceManager::updateWindowConfig(const std::string& filePath) {

    if (!windowInfo.updateRequired) {
        std::cerr << "did not saved file" << std::endl;
        return false;
    }
    std::cerr << "saved to file" << std::endl;

    // 1) read existing file
    std::ifstream in(filePath);
    if (!in.is_open()) return false;

    nlohmann::json j;
    try {
        in >> j;
    } catch (...) {
        return false;
    }
    in.close();

    // 2) modify only the fields you want
    // resolution
    j["resolution"] = { windowInfo.windowSize.x, windowInfo.windowSize.y };
    // simple booleans & ints
    j["fullscreen"]   = windowInfo.fullscreen;
    j["vertical sync"] = windowInfo.verticalSync;
    j["frame rate"]   = windowInfo.frameRate;

    // nested objects
    j["anti-aliasing level"]["enabled"] = windowInfo.antiAliasing;
    j["anti-aliasing level"]["level"]   = windowInfo.antiAliasingLevel;

    j["music"]["enabled"]    = windowInfo.music;
    j["music"]["level"]      = windowInfo.musicLevel;

    j["sound effect"]["enabled"] = windowInfo.soundEffect;
    j["sound effect"]["level"]   = windowInfo.soundLevel;

    // 3) write it back (pretty-printed)
    std::ofstream out(filePath);
    if (!out.is_open()) return false;
    out << std::setw(4) << j << std::endl;
    return !out.fail();
}


TowerData ResourceManager::getTowerData(const std::string& name) const {
    auto it = towerDataMap.find(name);
    if (it != towerDataMap.end()) {
        return it->second;
    }
    std::cerr << "Tower not found: " << name << std::endl;
    return TowerData{};
}


windowConfig& ResourceManager::getWindowData() {
    return windowInfo;
}


const gridConfig& ResourceManager::getGridData() const {
    return gridCfg;
}


sf::Vector2f ResourceManager::getScaling() {
    RECT work_area_rect;
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode(); // Full screen dimensions

    /** fullscreen case */
    if (windowInfo.fullscreen) {
        std::cout << "yes sir" << std::endl;
        return {windowInfo.windowSize.x / 1920.f, windowInfo.windowSize.y / 1080.f};
    }

    int titleBarHeight = GetSystemMetrics(SM_CYCAPTION);
    int borderWidth = GetSystemMetrics(SM_CXFRAME);
    int borderHeight = GetSystemMetrics(SM_CYFRAME);

    std::cout << "Title bar height: " << titleBarHeight << " px\n";
    std::cout << "Horizontal border width: " << borderWidth << " px\n";
    std::cout << "Vertical border height: " << borderHeight << " px\n";

    // 1) Auto-hide?
    bool autoHide = IsTaskbarAutoHide();
    if (autoHide) {
        if (desktop.size.y == windowInfo.windowSize.x) {
            return {windowInfo.windowSize.x - borderHeight / 1920.f, windowInfo.windowSize.y / 1080.f};
        }
        else {
            return {windowInfo.windowSize.x / 1920.f, windowInfo.windowSize.y / 1080.f};
        }
    }


    int height;
    // 2) Position & size
    APPBARDATA abd;
    if (GetTaskbarData(abd)) {
        // Determine edge
        const char* edgeName = "Unknown";
        switch (abd.uEdge) {
            case ABE_LEFT:   edgeName = "Left";   break;
            case ABE_TOP:    edgeName = "Top";    break;
            case ABE_RIGHT:  edgeName = "Right";  break;
            case ABE_BOTTOM: edgeName = "Bottom"; break;
        }

        // Compute width/height
        int width  = abd.rc.right  - abd.rc.left;
        height = abd.rc.bottom - abd.rc.top;

        // Thickness = the smaller dimension for that edge
        int thickness = (abd.uEdge == ABE_TOP || abd.uEdge == ABE_BOTTOM)
                        ? height
                        : width;

        std::cout << "Taskbar position: " << edgeName << "\n";
        std::cout << "Taskbar screen rect: "
                  << "{" << abd.rc.left << ", " << abd.rc.top << "} – "
                  << "{" << abd.rc.right << ", " << abd.rc.bottom << "}\n";
        std::cout << "Taskbar size: " << width << "x" << height << " px\n";
        std::cout << "Taskbar thickness: " << thickness << " px\n";
    }
    else {
        std::cout << "No taskbar detected via ABM_GETTASKBARPOS.\n";
    }


    float checkIfNeedAccount = desktop.size.y - height - titleBarHeight;

    float scaleX = windowInfo.windowSize.x / 1920.f;
    float scaleY;
    if (windowInfo.windowSize.y > checkIfNeedAccount) {
        scaleY = (windowInfo.windowSize.y - height - titleBarHeight) / 1080.f;
    }
    else {
        scaleY = windowInfo.windowSize.y / 1080.f;
    }

    return {scaleX, scaleY};
}


// Returns true if the taskbar is set to auto-hide
bool ResourceManager::IsTaskbarAutoHide() {
    APPBARDATA abd = { sizeof(abd) };
    UINT state = SHAppBarMessage(ABM_GETSTATE, &abd);
    return (state & ABS_AUTOHIDE) != 0;
}


// Fills abd with the taskbar’s screen rectangle and edge.
// Returns true if successful.
bool ResourceManager::GetTaskbarData(APPBARDATA& abd) {
    abd.cbSize = sizeof(abd);
    return SHAppBarMessage(ABM_GETTASKBARPOS, &abd) != 0;
}
