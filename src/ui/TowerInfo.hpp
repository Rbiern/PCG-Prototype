#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include "../core/ResourceManager.hpp"


class TowerInfo : public sf::Drawable {
public:
    explicit TowerInfo(const std::string& name);
    ~TowerInfo() override;
    void setPosition(sf::Vector2f position);
    void setScale(sf::Vector2f scale);

private:
    void draw(sf::RenderTarget& rt, sf::RenderStates) const override;
    void buildRangeSquares(sf::Vector2f scale);

    sf::Vector2f basePosition;
    TowerData towerData;
    // ** UI elements **
    sf::RectangleShape topBar;
    sf::RectangleShape iconBackground;
    sf::RectangleShape rangeBackground;
    sf::RectangleShape tag1Background;
    sf::RectangleShape tag2Background;
    sf::RectangleShape separationBar;
    std::vector<sf::RectangleShape> rangeSquares;
    std::array<sf::RectangleShape, 6> statNameBackground;
    std::array<sf::RectangleShape, 6> statValueBackground;
    sf::Texture classIconTexture;
    sf::Texture leftCornerTexture;
    sf::Texture rightCornerTexture;
    sf::Texture towerTexture;
    sf::Sprite* classIconSprite;
    sf::Sprite* leftCornerSprite;
    sf::Sprite* rightCornerSprite;
    sf::Sprite* towerSprite;
    sf::Text* nameText;
    sf::Text* rangeLabel;
    sf::Text* tag1Text;
    sf::Text* tag2Text;
    sf::Text* attributesLabel;
    std::array<sf::Text*, 6> statNameText;
    std::array<sf::Text*, 6> statValueText;
    constexpr static const std::array<sf::Vector2f, 6> namePos = {{{40,571},{40,602},{40,633}, {184,571},{184,602},{184,633}}};
    constexpr static const std::array<sf::Vector2f, 6> valPos  = {{{110,571},{110,602},{110,633}, {254,571},{254,602},{254,633}}};
};
