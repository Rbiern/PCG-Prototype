#include "Game.hpp"


Game::Game() : rm(ResourceManager::getInstance()) {
    // Load window configurations
    if (!rm.loadJsonConfig("../../config.json")) {
        std::cerr << "Creating Window Error: Cannot open JSON file.\n" << std::endl;
        exit(-1);
    }
    width = rm.resolution[0];
    height = rm.resolution[1];
    frameRate = rm.frameRate;
    antiAliasingEnabled = rm.antiAliasingEnabled;
    isFullscreen = rm.isFullscreen;
    verticalSync = rm.verticalSync;
    if (antiAliasingEnabled) {
        settings.antiAliasingLevel = rm.antiAliasingLevel;
    }

    // Create the window
    if (isFullscreen) {
        window.create(sf::VideoMode({width, height}),
                      title, sf::Style::Default,
                      sf::State::Fullscreen,
                      settings);
    } else {
        window.create(sf::VideoMode({width, height}),
                      title, sf::Style::Default,
                      sf::State::Windowed,
                      settings);
    }

    // Call these once creating the window
    window.setVerticalSyncEnabled(verticalSync);
    window.setFramerateLimit(frameRate);
    if (!icon.loadFromFile("../../images/icon.png")) {
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
    sf::Clock clock;
    double starting = 0.0001;

    // Game Loop: stops when window is not open
    while (window.isOpen()) {
        // Track frames per second info
        double ending = clock.getElapsedTime().asSeconds();
        double dt = ending - starting;
        starting = ending;
        // Display frames per second info in window bar
        std::string FPS = std::to_string(int (1 / dt));
        window.setTitle(title + " | FPS: " + FPS);

        // Process events
        while (const std::optional event = window.pollEvent()) {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
            // Handle other window input
            else
                 if (currentMenu->handleUserInput(*event)) window.close();
        }

        window.clear();             // Clear screen
        currentMenu->render(window);// Window state
        window.display();           // Update the window
    }
}
