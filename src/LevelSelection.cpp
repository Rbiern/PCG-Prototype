#include "LevelSelection.hpp"


LevelSelection::LevelSelection() {

}

bool LevelSelection::handleUserInput(const sf::Event &event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
            std::cout << "escape\n";
            return true;
        }
    } else if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {

        }
    }
    return false;
}


void LevelSelection::menuActionUpdate() {

}


void LevelSelection::render(sf::RenderWindow &window) {

}

