#include "SidePanel.hpp"


SidePanel::SidePanel() : rm(ResourceManager::getInstance()) {
    font = rm.getFont();

    panel.setSize({192.f, 720.f});
    panel.setFillColor(sf::Color(0x15, 0x12, 0x26));
    panel.setPosition({1088.f, 0.f});

    divider.setSize({6.f, 720.f});
    divider.setFillColor(sf::Color(0xD8, 0xA4, 0x0F));
    divider.setPosition({1088.f, 0.f});

    exitButton = rm.getTexture("../../images/exit.png");
    deleteButton = rm.getTexture("../../images/delete.png");
    directionButton = rm.getTexture("../../images/direction.png");
    coins = rm.getTexture("../../images/gold.png");
    heart = rm.getTexture("../../images/heart.png");
    remainingEnemies = rm.getTexture("../../images/e.png");

    deleteSprite = new sf::Sprite(deleteButton);
    deleteSprite->setPosition({1103.f, 263.f});

    directionSprite = new sf::Sprite(directionButton);
    directionSprite->setPosition({1151.f, 263.f});

    exitSprite = new sf::Sprite(exitButton);
    {
        sf::Vector2u size = exitButton.getSize();
        float scaleX = 152.f / size.x;
        float scaleY = 47.f  / size.y;
        exitSprite->setScale({scaleX, scaleY});
    }
    exitSprite->setPosition({1112.f, 22.f});

    coinsSprite = new sf::Sprite(coins);
    {
        sf::Vector2u size = coins.getSize();
        float scaleX = 40.f / size.x;
        float scaleY = 40.f / size.y;
        coinsSprite->setScale({scaleX, scaleY});
    }
    coinsSprite->setPosition({1110.f, 122.f});

    heartSprite = new sf::Sprite(heart);
    {
        sf::Vector2u size = heart.getSize();
        float scaleX = 38.f / size.x;
        float scaleY = 38.f / size.y;
        heartSprite->setScale({scaleX, scaleY});
    }
    heartSprite->setPosition({1108.f, 161.f});

    remainingEnemiesSprite = new sf::Sprite(remainingEnemies);
    {
        sf::Vector2u size = remainingEnemies.getSize();
        float scaleX = 38.f / size.x;
        float scaleY = 38.f / size.y;
        remainingEnemiesSprite->setScale({scaleX, scaleY});
    }
    remainingEnemiesSprite->setPosition({1108.f, 210.f});

    roundText = new sf::Text(font, "Round ", 42);
    roundText->setFillColor(sf::Color::Black);
    roundText->setOutlineColor(sf::Color::White);
    roundText->setOutlineThickness(2.f);
    roundText->setPosition({1132.f, 72.f});

    goldText = new sf::Text(font, " Gold", 24);
    goldText->setFillColor(sf::Color(0xFF, 0xD5, 0x4B));
    goldText->setPosition({1110.f + 40.f + 10.f, 122.f});

    healthText = new sf::Text(font, " Health", 24);
    healthText->setFillColor(sf::Color(0xEB, 0x29, 0x2B));
    healthText->setPosition({1108.f + 38.f + 10.f, 161.f});

    remainingText = new sf::Text(font, " Remaining\nEnemies", 24);
    remainingText->setFillColor(sf::Color(0xD8, 0x2A, 0xE8));
    remainingText->setPosition({1108.f + 38.f + 10.f, 202.f});

    isVisible = false;
}


SidePanel::~SidePanel() {
    delete exitSprite;
    delete deleteSprite;
    delete directionSprite;
    delete coinsSprite;
    delete heartSprite;
    delete remainingEnemiesSprite;
    delete roundText;
    delete goldText;
    delete healthText;
    delete remainingText;
}


void SidePanel::setRoundNumber(int stage) {
    roundText->setString("Round " + std::to_string(stage));
}


void SidePanel::setGoldNumber(int gold) {
    goldText->setString(std::to_string(gold) + " Gold");
}


void SidePanel::setHealthNumber(int health) {
   healthText->setString(std::to_string(health) + " Health");
}


void SidePanel::setRemainingNumber(int count) {
    remainingText->setString(std::to_string(count) + " Remaining\nEnemies");
}


void SidePanel::drawPanel(sf::RenderWindow &window) {
    window.draw(panel);
    window.draw(divider);
    window.draw(*exitSprite);
    window.draw(*coinsSprite);
    window.draw(*heartSprite);
    if (isVisible) {
        window.draw(*deleteSprite);
        window.draw(*directionSprite);
    }
    window.draw(*remainingEnemiesSprite);
    window.draw(*roundText);
    window.draw(*goldText);
    window.draw(*healthText);
    window.draw(*remainingText);
}
