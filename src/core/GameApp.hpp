#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include "../states/Menu.hpp"
#include "../states/StartUp.hpp"
#include "../states/Home.hpp"
#include "../states/Options.hpp"
#include "../states/Squad.hpp"
#include "../states/Creator.hpp"
#include "../states/Level.hpp"


class GameApp {
public:
    GameApp();                                      // Constructor
    ~GameApp();                                     // Destructor
    void setMenu(std::unique_ptr<Menu> menu);       // Setter if you want to change the current menu
    void executeGameApp();                          // Main loop function

private:
    // Window setup
    windowConfig* config;
    std::string title;
    sf::Image icon;
    sf::ContextSettings settings;
    sf::RenderWindow window;
    bool isFullscreen;
    // Window state and actions
    std::unique_ptr<Menu> currentMenu;
    sf::Clock clock;
    // Miscellaneous
    ResourceManager& rm;
};
