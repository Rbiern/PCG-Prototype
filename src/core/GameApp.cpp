#include "GameApp.hpp"

#include <iostream>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include "states/TitleScreen.hpp"
#include "ui/FpsCounter.hpp"


GameApp::GameApp()
    : appSettings(ASSET_CONFIG_PATH)
    , resourceManager(ASSET_MANIFEST_PATH)
    , soundEngine(appSettings.audio) {

    if (!appSettings.load()) {
        std::cerr << "Settings could not be loaded; using defaults" << std::endl;
    }
    if (!soundEngine.loadManifest(AUDIO_MANIFEST_PATH)) {
        std::cerr << "Audio manifest could not be loaded; starting without audio" << std::endl;
    }
    window.initializeWindow(appSettings.video);
    screenManager.pushState(new TitleScreen(makeScreenContext()));
}


GameApp::~GameApp() {
    if (appSettings.dirty && !appSettings.saveToDisk()) {
        std::cerr << "Could not save application settings" << std::endl;
    }

    if (window.renderWindow.isOpen()) {
        window.renderWindow.close();
    }

    soundEngine.stopAll();
}


ScreenContext GameApp::makeScreenContext() {
    return {
        resourceManager,
        screenManager,
        soundEngine,
        [this] { closeApp(); },
        [this] { toggleFullscreen(); },
    };
}


void GameApp::toggleFullscreen() {
    appSettings.dirty = true;
    window.toggleFullScreen();
}


void GameApp::closeApp() {
    closeRequested = true;
}


int GameApp::execute() {
    FpsCounter fpsCounter(resourceManager.getFont("debug"));
    screenManager.stateTransition();
    soundEngine.update(0.f);

    if (!screenManager.activeScreen()) {
        std::cerr << "Error: Game started with no states!" << std::endl;
        return -1;
    }

    while (window.renderWindow.isOpen()) {
        Screen* screen = screenManager.activeScreen();
        if (screen == nullptr) {
            break;
        }

        const FrameTime time = timeCalculator.tick();

        if (appSettings.gameplay.showFrameRate) {
            fpsCounter.update(time.unscaledDelta);
        }

        while (const auto event = window.renderWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.renderWindow.close();
                closeRequested = true;
                break;
            }

            if (event->is<sf::Event::Resized>()) {
                window.updateView();
                continue;
            }

            const sf::Vector2i mousePixel = sf::Mouse::getPosition(window.renderWindow);
            const sf::Vector2f mouseWorld = window.renderWindow.mapPixelToCoords(mousePixel);
            screen->handleEvent(*event, mouseWorld);
        }

        if (closeRequested) {
            break;
        }

        screen->update(time);
        soundEngine.setListenerPosition(window.renderWindow.getView().getCenter());
        soundEngine.update(time.unscaledDelta);

        screen->render(window.renderWindow);
        if (appSettings.gameplay.showFrameRate) {
            window.renderWindow.draw(fpsCounter);
        }
        window.renderWindow.display();

        screenManager.stateTransition();
    }

    return 0;
}
