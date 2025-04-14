#include "StartUp.hpp"


StartUp::StartUp() : rm(ResourceManager::getInstance()) {
    background = rm.getTexture("../../images/Loading Screen.png");

    startButton.setSize(sf::Vector2f(320, 17));
    float height = (720.f * 0.962f) - startButton.getSize().y;
    float width = (1280.f * 0.5f) - (startButton.getSize().x / 2.f);

    startButton.setPosition(sf::Vector2f(width, height));
    startButton.setFillColor(sf::Color(0x26, 0xD5, 0x5E));
}


bool StartUp::handleUserInput(const sf::Event &event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
            std::cout << "escape\n";
            return true;
        }
    } else if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
            if (startButton.getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y))) {
                std::cout << "Button was clicked!" << std::endl;
                game->setMenu(std::make_unique<LevelSelection>());
            }
        }
    }
    return false;
}


void StartUp::menuActionUpdate() {
    if (flashClock.getElapsedTime().asMilliseconds() >= 650) {
        flash = !flash;
        flashClock.restart();

        if (flash)
            startButton.setFillColor(sf::Color::White); // flash white
        else
            startButton.setFillColor(sf::Color(0x26, 0xD5, 0x5E)); // return to green
    }
}


void StartUp::render(sf::RenderWindow &window) {
    sf::Sprite sprite(background);
    window.draw(sprite);
    window.draw(startButton);
}
