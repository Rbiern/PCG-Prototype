#pragma once
#include <SFML/Graphics.hpp>
#include "../core/ResourceManager.hpp"


class LevelHUD {
    friend class Level;
public:
    LevelHUD();
    ~LevelHUD();
    void enablePopUp();
    void drawHUD(sf::RenderWindow &window);

private:
    sf::RectangleShape overlay;

    sf::Texture pauseButton;
    sf::Texture popUp;
    sf::Texture yesButton;
    sf::Texture noButton;

    sf::Sprite* pauseButtonSprite;
    sf::Sprite* popUpSprite;
    sf::Sprite* yesButtonSprite;
    sf::Sprite* noButtonSprite;

    bool enableFlag;
    ResourceManager& rm;
};
