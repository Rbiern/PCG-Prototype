#include "StartUp.hpp"


StartUp::StartUp() : rm(ResourceManager::getInstance()) {
    background = rm.getTexture("../../images/Loading Screen.png");

    startButton.setSize(sf::Vector2f(320, 30));
    float height = (720.f * 0.962f) - startButton.getSize().y;
    float width = (1280.f * 0.5f) - (startButton.getSize().x / 2.f);

    startButton.setPosition(sf::Vector2f(width, height));
    startButton.setFillColor(sf::Color(0x26, 0xD5, 0x5E));

    flash = false;
    flashTimer = 0.f;
}


bool StartUp::handleUserInput(const sf::Event &event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
            return true;
        }
    } else if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
            if (startButton.getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y))) {
                game->setMenu(std::make_unique<LevelSelection>());
                return false;
            }
        }
    }
    return false;
}


void StartUp::menuActionUpdate(float delta) {
    flashTimer += delta;

    if (flashTimer >= 0.65f) { // 650ms or 0.65 seconds
        flash = !flash;
        flashTimer = 0.f;
    }
    if (flash) {
        startButton.setFillColor(sf::Color::White);
    }else {
        startButton.setFillColor(sf::Color(0x26, 0xD5, 0x5E));
    }
}


void StartUp::render(sf::RenderWindow &window) {
    sf::Sprite sprite(background);
    window.draw(sprite);
    window.draw(startButton);
}
