#pragma once
#include <SFML/Graphics.hpp>
#include "../core/ResourceManager.hpp"


class PlayerCard {
public:
    PlayerCard(float baseX, float baseY, int cost, sf::Color color);
    ~PlayerCard();
    void drawPlayerCard(sf::RenderWindow& window);

    sf::Font font;
    sf::Text* costText;

    sf::RectangleShape backgroundRect;
    sf::RectangleShape topLine;
    sf::RectangleShape redLine;
    sf::RectangleShape whiteBox1;
    sf::RectangleShape whiteBox2;
    sf::RectangleShape blackBox;
    sf::RectangleShape towerShape;

    sf::Texture towerClass;
    sf::Texture tower;

private:
    ResourceManager& rm;
};
