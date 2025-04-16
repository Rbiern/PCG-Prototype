#include "LevelHUD.hpp"


LevelHUD::LevelHUD() : rm(ResourceManager::getInstance()) {
    pauseButton = rm.getTexture("../../images/pause.png");
    towerCountIcon = rm.getTexture("../../images/tower.png");
    remainingEnemiesIcon = rm.getTexture("../../images/e.png");
    font = rm.getFont();

    towerCountText = new sf::Text(font, "Tower Count: ", 26);
    towerCountText->setStyle(sf::Text::Bold);
    towerCountText->setFillColor(sf::Color::Black);
    towerCountText->setOutlineColor(sf::Color::White);
    towerCountText->setOutlineThickness(1.f);
    towerCountText->setPosition({310.f + 33.f + 10.f, 22.f});

    remainingText = new sf::Text(font, "Remaining Enemies: ", 26);
    remainingText->setStyle(sf::Text::Bold);
    remainingText->setFillColor(sf::Color::Black);
    remainingText->setOutlineColor(sf::Color::White);
    remainingText->setOutlineThickness(1.f);
    remainingText->setPosition({542.f + 30.f + 10.f, 22.f});


    pauseButtonSprite = new sf::Sprite(pauseButton);
    {
        sf::Vector2u size = pauseButton.getSize();
        float scaleX = 50.f / size.x;
        float scaleY = 50.f / size.y;
        pauseButtonSprite->setScale({scaleX, scaleY});
    }
    pauseButtonSprite->setPosition({25.f, 25.f});

    towerCountIconSprite = new sf::Sprite(towerCountIcon);
    {
        sf::Vector2u size = towerCountIcon.getSize();
        float scaleX = 33.f / size.x;
        float scaleY = 33.f / size.y;
        towerCountIconSprite->setScale({scaleX, scaleY});
    }
    towerCountIconSprite->setPosition({310.f, 22.f});

    remainingEnemiesIconSprite = new sf::Sprite(remainingEnemiesIcon);
    {
        sf::Vector2u size = remainingEnemiesIcon.getSize();
        float scaleX = 30.f / size.x;
        float scaleY = 30.f / size.y;
        remainingEnemiesIconSprite->setScale({scaleX, scaleY});
    }
    remainingEnemiesIconSprite->setPosition({542.f, 25.f});


    divider.setSize({6.f, 33.f});
    divider.setFillColor(sf::Color::White);
    divider.setPosition({526.f, 22.f});
}


LevelHUD::~LevelHUD() {
    delete pauseButtonSprite;
    delete towerCountIconSprite;
    delete remainingEnemiesIconSprite;
    delete towerCountText;
    delete remainingText;
}


void LevelHUD::setTowerCountText(int count) {
    towerCountText->setString("Tower Count: " + std::to_string(count));
}


void LevelHUD::setRemainingText(int count) {
    remainingText->setString("Remaining Enemies: " + std::to_string(count));
}


void LevelHUD::drawHUD(sf::RenderWindow &window) {
    window.draw(*pauseButtonSprite);
    window.draw(*towerCountIconSprite);
    window.draw(*remainingEnemiesIconSprite);
    window.draw(divider);
    window.draw(*towerCountText);
    window.draw(*remainingText);
}
