#include "GameApp.hpp"

#include <iostream>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include "states/Screen.hpp"
#include "states/TitleScreen.hpp"


GameApp::GameApp()
    : appSettings(ASSET_CONFIG_PATH)
    , resourceManager(ASSET_MANIFEST_PATH) {
    window.initializeWindow(appSettings);
    screenManager.pushState(new TitleScreen(makeScreenContext()));
}


GameApp::~GameApp() {
    if (appSettings.dirty && !appSettings.saveToDisk()) {
        std::cerr << "Could not save application settings" << std::endl;
    }

    if (window.renderWindow.isOpen()) {
        window.renderWindow.close();
    }
}


ScreenContext GameApp::makeScreenContext() {
    return {
        resourceManager,
        screenManager,
        [this] { closeApp(); },
        [this] { toggleFullscreen(); }
    };
}


void GameApp::toggleFullscreen() {
    window.toggleFullScreen();
}


void GameApp::closeApp() {
    closeRequested = true;
}


int GameApp::execute() {
    screenManager.stateTransition();

    if (!screenManager.activeScreen()) {
        std::cerr << "Error: Game started with no states!" << std::endl;
        return -1;
    }

    sf::RenderWindow* renderWindow = &window.renderWindow;

    while (renderWindow->isOpen()) {
        Screen* screen = screenManager.activeScreen();
        if (screen == nullptr) {
            break;
        }

        const FrameTime time = timeCalculator.tick();

        while (const auto event = renderWindow->pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                renderWindow->close();
                closeRequested = true;
                break;
            }

            if (event->is<sf::Event::Resized>()) {
                window.updateView();
                continue;
            }

            const sf::Vector2i mousePixel = sf::Mouse::getPosition(*renderWindow);
            const sf::Vector2f mouseWorld = renderWindow->mapPixelToCoords(mousePixel);

            screen->handleEvent(*event, mouseWorld);
        }

        if (closeRequested) {
            break;
        }

        screen->update(time);
        renderWindow->clear();
        screen->render(*renderWindow);
        renderWindow->display();

        screenManager.stateTransition();
    }

    return 0;
}
