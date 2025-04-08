#ifndef PCG_PROTOTYPE_GAME_HPP
#define PCG_PROTOTYPE_GAME_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "ResourceManager.hpp"
#include "Menu.hpp"
#include "StartUp.hpp"
#include "LevelSelection.hpp"
#include "Level.hpp"
#pragma once

class Game {
private:
    // Window setup
    sf::RenderWindow window;
    std::string title = "Thesis";
    sf::Image icon;
    unsigned int width;
    unsigned int height;
    unsigned int frameRate;
    bool isFullscreen;
    bool verticalSync;

    // Window state
    std::unique_ptr<Menu> currentMenu;

    // Other
    ResourceManager& rm;

public:
    Game();                                         // Constructor
    ~Game();                                        // Destructor
    void setMenu(std::unique_ptr<Menu> menu);       // Setter if you want to change the current menu
    void executeGame();                             // Main loop function
};

#endif
