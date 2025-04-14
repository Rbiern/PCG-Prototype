#include "LevelSelection.hpp"


LevelSelection::LevelSelection(): rm(ResourceManager::getInstance()) {
    background = rm.getTexture("../../images/Home.png");
    exitButton = rm.getTexture("../../images/exit.png");

    exitButtonSprite = new sf::Sprite(exitButton);
    exitButtonSprite->setPosition({1094.f, 35.f});

    playButton.setSize(sf::Vector2f(100, 100));
    float height = (720.f * 1.f) - playButton.getSize().y;
    float width = (1280.f * 0.5f) - (playButton.getSize().x / 2.f);
    playButton.setPosition(sf::Vector2f(width, height));
    playButton.setFillColor(sf::Color(0x26, 0xD5, 0x5E));
}


LevelSelection::~LevelSelection() {
    delete exitButtonSprite;
}


bool LevelSelection::handleUserInput(const sf::Event &event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
            return true;
        }
    } else if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
            if (exitButtonSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y))) {
                game->setMenu(std::make_unique<StartUp>());
            }
            if (playButton.getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y))) {
                game->setMenu(std::make_unique<Level>());
            }
        }
    }
    return false;
}


void LevelSelection::menuActionUpdate() {

}


void LevelSelection::render(sf::RenderWindow &window) {
    sf::Sprite sprite(background);
    window.draw(sprite);
    window.draw(playButton);
    window.draw(*exitButtonSprite);
}
