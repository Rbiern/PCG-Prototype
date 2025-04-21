#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include "../states/Menu.hpp"
#include "../states/StartUp.hpp"
#include "../states/LevelSelection.hpp"
#include "../states/Level.hpp"


class Game {
public:
    Game();                                         // Constructor
    ~Game();                                        // Destructor
    void setMenu(std::unique_ptr<Menu> menu);       // Setter if you want to change the current menu
    void executeGame();                             // Main loop function

private:
    // Window setup
    sf::RenderWindow window;
    sf::ContextSettings settings;
    std::string title;
    sf::Image icon;
    unsigned int width;
    unsigned int height;
    unsigned int frameRate;
    bool antiAliasingEnabled;
    bool isFullscreen;
    bool verticalSync;

    // Window state and actions
    std::unique_ptr<Menu> currentMenu;
    sf::Clock deltaTime;
    bool pauseFlag = false;

    // Other
    ResourceManager& rm;
};
