#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include "Menu.hpp"
#include "StartUp.hpp"
#include "LevelSelection.hpp"
#include "Level.hpp"


class Game {
private:
    // Window setup
    sf::RenderWindow window;
    sf::ContextSettings settings;
    std::string title = "Thesis";
    sf::Image icon;
    unsigned int width;
    unsigned int height;
    unsigned int frameRate;
    bool antiAliasingEnabled;
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
