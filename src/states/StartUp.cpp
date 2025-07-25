#include "StartUp.hpp"


StartUp::StartUp() : rm(ResourceManager::getInstance()) {
    flash = false;
    flashTimer = 0.f;
    backgroundTexture = rm.getTexture("../../images/pages/start_page.png");
    backgroundSprite = new sf::Sprite(backgroundTexture);
    backgroundSprite->setPosition(sf::Vector2f{0.f, 0.f});
    startButton.setFillColor(sf::Color(0x26, 0xD5, 0x5E));
    resize(rm.ResourceManager::getScaling());
}


StartUp::~StartUp() {
    delete backgroundSprite;
}


bool StartUp::handleUserInput(const sf::Event &event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
            return true;
        }
    } else if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
            if (startButton.getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position))) {
                game->setMenu(std::make_unique<Home>());
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
    } else {
        startButton.setFillColor(sf::Color(0x26, 0xD5, 0x5E));
    }
}


void StartUp::resize(sf::Vector2f scale) {
    backgroundSprite->setScale(scale);
    startButton.setSize(sf::Vector2f{477.f * scale.x, 42.f * scale.y});
    startButton.setPosition(sf::Vector2f{721.f * scale.x, 996.f * scale.y});
}


void StartUp::render(sf::RenderWindow &window) {
    window.draw(*backgroundSprite);
    window.draw(startButton);
}
