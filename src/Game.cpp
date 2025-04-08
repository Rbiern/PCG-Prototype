#include "Game.hpp"


Game::Game() : rm(ResourceManager::getInstance()) {
    // Load window configurations
    if (!rm.loadJsonConfig("../config.json")) {
        std::cerr << "Creating Window Error: Cannot open JSON file.\n" << std::endl;
        exit(-1);
    }
    width = rm.resolution[0];
    height = rm.resolution[1];
    frameRate = rm.frameRate;
    isFullscreen = rm.isFullscreen;
    verticalSync = rm.verticalSync;

    // Create the window
    if (isFullscreen) {
        window.create(sf::VideoMode({width, height}),
                      title, sf::Style::Default,
                      sf::State::Fullscreen);
    } else {
        window.create(sf::VideoMode({width, height}),
                      title, sf::Style::Default,
                      sf::State::Windowed);
    }

    // Call these once creating the window
    window.setVerticalSyncEnabled(verticalSync);
    window.setFramerateLimit(frameRate);
    if (!icon.loadFromFile("../images/icon.png")) {
        std::cerr << "Load Icon image error.\n" << std::endl;
    }
    window.setIcon(icon);

    // Set window state
    setMenu(std::make_unique<StartUp>());
    currentMenu->setGame(this);
}


Game::~Game() {
    if (window.isOpen()) {
        window.close();
    }
}


void Game::setMenu(std::unique_ptr<Menu> menu) {
    // Transfer ownership of 'menu' to 'currentMenu'
    currentMenu = std::move(menu);
    if (currentMenu) {
        currentMenu->setGame(this);
    }
}


void Game::executeGame() {
    // Game Loop: stops when window is not open
    while (window.isOpen()) {
        // Process events
        while (const std::optional event = window.pollEvent()) {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
            // Handle other window input
            else
                currentMenu->handleUserInput(*event);
        }

        window.clear();             // Clear screen
        currentMenu->render(window);// Window state
        window.display();           // Update the window
    }
}