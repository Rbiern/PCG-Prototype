#pragma once
#include <SFML/Graphics.hpp>
#include "../core/ResourceManager.hpp"


class SidePanel {
public:
    SidePanel();
    ~SidePanel();
    void setRound(int stage);
    void drawSelectionPanel(sf::RenderWindow &window);

    // Side Panel
    sf::Font font;
    sf::Text* roundText;
    sf::Text* goldText;
    sf::Text* healthText;

    sf::RectangleShape panel;
    sf::RectangleShape divider;

    sf::Texture exitButton;
    sf::Texture coins;
    sf::Texture heart;

    sf::Sprite* exitSprite;
    sf::Sprite* coinsSprite;
    sf::Sprite* heartSprite;

private:
    ResourceManager& rm;
};
