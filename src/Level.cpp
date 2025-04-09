#include "Level.hpp"


Level::Level() {

}

bool Level::handleUserInput(const sf::Event &event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
            std::cout << "escape\n";
            game->setMenu(std::make_unique<LevelSelection>());
        }
    } else if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {

    }
    return false;
}


void Level::menuActionUpdate() {

}


void Level::render(sf::RenderWindow &window) {

}
