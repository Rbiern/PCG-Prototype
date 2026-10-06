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
    appSettings.load();
    window.initializeWindow(appSettings);

    pushState(new TitleScreen(*this));
}


GameApp::~GameApp() {
    if (appSettings.dirty) {
        bool flag = appSettings.saveToDisk();
    }

    if (window.renderWindow.isOpen()) {
        window.renderWindow.close();
    }

    for (Screen*& menu : stateStack) {
        delete menu;
    }
    stateStack.clear();

    while (!transitions.empty()) {
        delete transitions.front().state;
        transitions.pop();
    }
}


void GameApp::pushState(Screen* state) {
    transitions.push({TransitionType::Push, state});
}


void GameApp::popState() {
    transitions.push({TransitionType::Pop, nullptr});
}


void GameApp::switchState(Screen* state) {
    transitions.push({TransitionType::Switch, state});
}


void GameApp::close() {
    transitions.push({TransitionType::Close, nullptr});
}


ResourceManager& GameApp::resources() {
    return resourceManager;
}


void GameApp::toggleFullScreen() {
    window.toggleFullScreen();
}


int GameApp::execute() {
    stateTransition();

    if (stateStack.empty()) {
        std::cerr << "Error: Game started with no states!" << std::endl;
        return -1;
    }

    sf::RenderWindow* renderWindow = &window.renderWindow;

    while (renderWindow->isOpen() && !stateStack.empty()) {
        const FrameTime time = timeCalculator.tick();

        while (const auto event = renderWindow->pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                renderWindow->close();
                break;
            }

            if (event->is<sf::Event::Resized>()) {
                window.updateView();
                continue;
            }

            const sf::Vector2i mousePixel = sf::Mouse::getPosition(*renderWindow);
            const sf::Vector2f mouseWorld = renderWindow->mapPixelToCoords(mousePixel);

            if (stateStack.back()->handleUserInput(*this, *event, mouseWorld)) {
                renderWindow->close();
                break;
            }
        }

        if (!renderWindow->isOpen()) {
            break;
        }

        renderWindow->clear();
        stateStack.back()->update(time);
        stateStack.back()->render(*renderWindow);
        renderWindow->display();

        stateTransition();
    }

    return 0;
}


void GameApp::stateTransition() {
    while (!transitions.empty()) {
        auto&[type, scene] = transitions.front();

        switch (type) {
            case TransitionType::Push:
                stateStack.push_back(scene);
                break;

            case TransitionType::Pop:
                if (!stateStack.empty()) {
                    const Screen* previous = stateStack.back();
                    delete previous;
                    stateStack.pop_back();
                }
                break;

            case TransitionType::Switch:
                for (Screen*& menu : stateStack) {
                    delete menu;
                }
                stateStack.clear();
                if (scene != nullptr) {
                    stateStack.push_back(scene);
                }
                break;

            case TransitionType::Close:
                window.renderWindow.close();
                break;
        }

        transitions.pop();
    }
}
