#pragma once
#include <SFML/Graphics.hpp>
#include "../core/ResourceManager.hpp"


class SidePanel {
    friend class Level;
public:
    SidePanel();
    ~SidePanel();
    void setRoundNumber(int stage);
    void setGoldNumber(int gold);
    void setHealthNumber(int health);
    void setRemainingNumber(int count);
    void drawPanel(sf::RenderWindow &window);

private:
    sf::Font font;
    sf::Text* roundText;
    sf::Text* goldText;
    sf::Text* healthText;
    sf::Text* remainingText;

    sf::RectangleShape panel;
    sf::RectangleShape divider;

    sf::Texture exitButton;
    sf::Texture deleteButton;
    sf::Texture directionButton;
    sf::Texture coins;
    sf::Texture heart;
    sf::Texture remainingEnemies;

    sf::Sprite* exitSprite;
    sf::Sprite* deleteSprite;
    sf::Sprite* directionSprite;
    sf::Sprite* coinsSprite;
    sf::Sprite* heartSprite;
    sf::Sprite* remainingEnemiesSprite;

    bool isVisible;

    ResourceManager& rm;
};
