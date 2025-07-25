#include "GameApp.hpp"


GameApp::GameApp() : rm(ResourceManager::getInstance()) {
    // Load window configurations from json file
    if (!rm.loadJsonConfig("../../config.json")) {
        std::cerr << "Creating Window Error: Cannot open JSON file.\n" << std::endl;
        exit(-1);
    }
    config = &rm.getWindowData();
    title = "Prototype";
    isFullscreen = config->fullscreen;
    if (config->antiAliasing) {
        settings.antiAliasingLevel = config->antiAliasingLevel;
    }

    // Create window
    if (config->fullscreen) {
        window.create(sf::VideoMode::getDesktopMode(),
                      title,
                      sf::Style::Default,
                      sf::State::Fullscreen,
                      settings);
    }
    else {
        window.create(sf::VideoMode(config->windowSize),
                      title,
                      sf::Style::Default,
                      sf::State::Windowed,
                      settings);
    }

    if (config->verticalSync) {
        window.setVerticalSyncEnabled(true);
    } else {
        window.setFramerateLimit(config->frameRate);
    }

    if (!icon.loadFromFile("../../images/icon.png")) {
        std::cerr << "Load Icon image error." << std::endl;
    } else {
        window.setIcon(icon);
    }

    // Set window state
    setMenu(std::make_unique<StartUp>());
    currentMenu->setGame(this);
}


GameApp::~GameApp() {
    if (window.isOpen()) {
        window.close();
    }
}


void GameApp::setMenu(std::unique_ptr<Menu> menu) {
    currentMenu = std::move(menu);
    if (currentMenu) {
        currentMenu->setGame(this);
    }
}


void GameApp::executeGameApp() {
    if (!rm.loadTowerData("../../towers.json")) {
        std::cerr << "Creating UI Error: Cannot open JSON file.\n" << std::endl;
        exit(-1);
    }
    float deltaTime;


    // Game Loop: stops when window is not open
    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();


        // Process input events
        while (const std::optional event = window.pollEvent()) {
            // Close window: exit
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            // Resize window: scale
            else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                config->windowSize = resized->size;
                window.setView(sf::View(sf::FloatRect({0, 0}, {static_cast<float>(resized->size.x), static_cast<float>(resized->size.y)})));
                sf::Vector2f newScale = sf::Vector2f{float(resized->size.x) / 1920.f, float(resized->size.y) / 1080.f};
                currentMenu->resize(newScale);
            }
            // Fullscreen mode
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scan::F11) {
                    config->fullscreen = !isFullscreen;
                    sf::RenderWindow desktop;
                    (isFullscreen = !isFullscreen) ? window.create(sf::VideoMode(config->windowSize), title, sf::Style::Default, sf::State::Fullscreen, settings) : window.create(sf::VideoMode(config->windowSize), title, sf::Style::Default, sf::State::Windowed, settings);
                    window.setView(sf::View(sf::FloatRect({0, 0}, {static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)})));
                    window.setVerticalSyncEnabled(config->verticalSync);
                    window.setFramerateLimit(config->frameRate);
                    window.setIcon(icon);
                    currentMenu->resize({window.getSize().x / 1920.f, window.getSize().y / 1080.f});
                }
            }
            // Other inputs
            if (currentMenu->handleUserInput(*event)) {
                window.close();
            }
        }


        window.clear();                             // Clear screen
        currentMenu->menuActionUpdate(deltaTime);   // Move in game time forward
        currentMenu->render(window);                // Window state
        window.display();                           // Update the window
    }
    rm.updateWindowConfig("../../config.json");
}
