#pragma once
#include <SFML/Graphics.hpp>
#include "../core/ResourceManager.hpp"


class PlayerCard : public sf::Drawable {
public:
    explicit PlayerCard(const std::string& id);
    ~PlayerCard() override;
    void setPosition(sf::Vector2f position);
    sf::Vector2f getPosition();
    void setScale(sf::Vector2f scale);
    bool contains(sf::Vector2f mousePos) const;
    TowerData towerData;

private:
    void draw(sf::RenderTarget& rt, sf::RenderStates) const override;

    sf::Vector2f basePosition;
    sf::RectangleShape hitBox;
    sf::RectangleShape overlay;
    sf::RectangleShape topBar;
    sf::RectangleShape bottomBar;
    sf::RectangleShape classTopBar;
    sf::RectangleShape classBottomBar;
    sf::RectangleShape iconBackground;
    sf::RectangleShape costBackground;
    sf::Texture classIconTexture;
    sf::Texture towerTexture;
    sf::Sprite* classIconSprite;
    sf::Sprite* towerSprite;
    sf::Text* costText;
};
