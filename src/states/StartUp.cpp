#include "StartUp.hpp"


StartUp::StartUp() : rm(ResourceManager::getInstance()) {
    if (!texture.loadFromFile("../../images/Loading Screen.png", false, sf::IntRect({0, 0}, {1280, 720})))
        std::cout << "Error" << std::endl;

    shape.setSize(sf::Vector2f(320, 17));
    float height = (720.f * 0.962f) - shape.getSize().y;
    float width = (1280.f * 0.5f) - (shape.getSize().x / 2.f);

    shape.setPosition(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color(0x26, 0xD5, 0x5E));
}


bool StartUp::handleUserInput(const sf::Event &event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
            std::cout << "escape\n";
            return true;
        }
    } else if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
            if (shape.getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y))) {
                game->setMenu(std::make_unique<LevelSelection>());
            }
        }
    }
    return false;
}


void StartUp::render(sf::RenderWindow &window) {
    sf::Sprite sprite(texture);
    window.draw(sprite);
    window.draw(shape);
}


void StartUp::menuActionUpdate() {

}
