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
    coins = rm.getTexture("../../images/gold.png");
    heart = rm.getTexture("../../images/heart.png");

    exitSprite = new sf::Sprite(exitButton);
    {
        sf::Vector2u texSize = exitButton.getSize();
        float scaleX = 152.f / texSize.x;
        float scaleY = 47.f  / texSize.y;
        exitSprite->setScale({scaleX, scaleY});
    }
    exitSprite->setPosition({1112.f, 22.f});

    coinsSprite = new sf::Sprite(coins);
    {
        sf::Vector2u texSize = coins.getSize();
        float scaleX = 40.f / texSize.x;
        float scaleY = 40.f / texSize.y;
        coinsSprite->setScale({scaleX, scaleY});
    }
    coinsSprite->setPosition({1110.f, 122.f});

    heartSprite = new sf::Sprite(heart);
    {
        sf::Vector2u texSize = heart.getSize();
        float scaleX = 38.f / texSize.x;
        float scaleY = 38.f / texSize.y;
        heartSprite->setScale({scaleX, scaleY});
    }
    heartSprite->setPosition({1108.f, 161.f});


    roundText = new sf::Text(font, "Round 1", 42);
    roundText->setFillColor(sf::Color::Black);
    roundText->setOutlineColor(sf::Color::White);
    roundText->setOutlineThickness(2.f);
    roundText->setPosition({1132.f, 72.f});

    goldText = new sf::Text(font, "85 Gold", 24);
    goldText->setFillColor(sf::Color(0xFF, 0xD5, 0x4B));
    goldText->setPosition({1110.f + 40.f + 10.f, 122.f});

    healthText = new sf::Text(font, "200 Health", 24);
    healthText->setFillColor(sf::Color(0xEB, 0x29, 0x2B));
    healthText->setPosition({1108.f + 38.f + 10.f, 161.f});
}


SidePanel::~SidePanel() {
    delete exitSprite;
    delete coinsSprite;
    delete heartSprite;
    delete roundText;
    delete goldText;
    delete healthText;
}


void SidePanel::setRound(int stage) {
    roundText->setString("Round " + std::to_string(stage));
}


void SidePanel::drawSelectionPanel(sf::RenderWindow &window) {
    window.draw(panel);
    window.draw(divider);
    window.draw(*exitSprite);
    window.draw(*coinsSprite);
    window.draw(*heartSprite);
    window.draw(*roundText);
    window.draw(*goldText);
    window.draw(*healthText);
}
