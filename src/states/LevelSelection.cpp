#include "LevelSelection.hpp"


LevelSelection::LevelSelection(): rm(ResourceManager::getInstance()) {
    background = rm.getTexture("../../images/Home.png");
    exitButton = rm.getTexture("../../images/exit.png");
    playButton = rm.getTexture("../../images/playbutton.png");

    backgroundSprite = new sf::Sprite(background);
    backgroundSprite->setPosition({0, 0});

    exitButtonSprite = new sf::Sprite(exitButton);
    {
        sf::Vector2u size = exitButton.getSize();
        float scaleX = 152.f / size.x;
        float scaleY = 47.f  / size.y;
        exitButtonSprite->setScale({scaleX, scaleY});
    }
    exitButtonSprite->setPosition({1094.f, 35.f});

    playButtonSprite = new sf::Sprite(playButton);
    {
        sf::Vector2u size = playButton.getSize();
        float scaleX = 152.f / size.x;
        float scaleY = 47.f  / size.y;
        playButtonSprite->setScale({scaleX, scaleY});
    }
    playButtonSprite->setPosition({565, 600});
}


LevelSelection::~LevelSelection() {
    delete backgroundSprite;
    delete exitButtonSprite;
    delete playButtonSprite;
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
                return false;
            }
            if (playButtonSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y))) {
                game->setMenu(std::make_unique<Level>());
                return false;
            }
        }
    }
    return false;
}


void LevelSelection::menuActionUpdate(float delta) { }


void LevelSelection::render(sf::RenderWindow &window) {
    window.draw(*backgroundSprite);
    window.draw(*playButtonSprite);
    window.draw(*exitButtonSprite);
}
