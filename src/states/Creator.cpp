#include "Creator.hpp"


Creator::Creator() : rm(ResourceManager::getInstance()) {
    background = rm.getTexture("../../images/c.png");
    homeButton = rm.getTexture("../../images/buttons/home_button.png");

    backgroundSprite = new sf::Sprite(background);
    backgroundSprite->setPosition({0, 0});

    homeButtonSprite = new sf::Sprite(homeButton);
    {
        sf::Vector2u size = homeButton.getSize();
        float scaleX = 152.f / size.x;
        float scaleY = 47.f  / size.y;
        homeButtonSprite->setScale({scaleX, scaleY});
    }
    homeButtonSprite->setPosition({1070.f, 35.f});
}


Creator::~Creator() {
    delete backgroundSprite;
    delete homeButtonSprite;
}


bool Creator::handleUserInput(const sf::Event &event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
            game->setMenu(std::make_unique<Home>());
            return false;
        }
    }
    else if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
            if (homeButtonSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y))) {
                game->setMenu(std::make_unique<Home>());
                return false;
            }
        }
    }
    return false;
}


void Creator::menuActionUpdate(float delta) {

}


void Creator::render(sf::RenderWindow &window) {
    window.draw(*backgroundSprite);
    window.draw(*homeButtonSprite);
}

void Creator::resize(sf::Vector2f scale) {

}
