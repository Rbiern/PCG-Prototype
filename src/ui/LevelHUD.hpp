#pragma once
#include <SFML/Graphics.hpp>
#include "../core/ResourceManager.hpp"


class LevelHUD {
public:
    LevelHUD();
    ~LevelHUD();
    void setTowerCountText(int count);
    void setRemainingText(int count);
    void drawHUD(sf::RenderWindow &window);

    sf::Font font;
    sf::Text* towerCountText;
    sf::Text* remainingText;
    sf::RectangleShape divider;
    sf::Texture pauseButton;
    sf::Texture towerCountIcon;
    sf::Texture remainingEnemiesIcon;
    sf::Sprite* pauseButtonSprite = nullptr;
    sf::Sprite* towerCountIconSprite = nullptr;
    sf::Sprite* remainingEnemiesIconSprite = nullptr;

private:
    ResourceManager& rm;
};
