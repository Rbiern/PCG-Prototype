#pragma once
#include <SFML/Graphics.hpp>
#include "../core/ResourceManager.hpp"


class PlayerCard {
    friend class Level;
public:
    PlayerCard(float baseX, float baseY, const std::string& name);
    ~PlayerCard();
    void drawPlayerCard(sf::RenderWindow& window);

private:
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

    std::string imagePath;
    ResourceManager& rm;
};
