#pragma once
#include <SFML/Graphics.hpp>
#include "../core/ResourceManager.hpp"


class SelectionButton : public sf::Drawable {
public:
    explicit SelectionButton(const std::string& labelText);
    ~SelectionButton() override;
    void updateColors();
    void setPosition(sf::Vector2f position);
    sf::Vector2f getPosition();
    void setScale(sf::Vector2f scale);
    bool contains(sf::Vector2f mousePos) const;

private:
    void draw(sf::RenderTarget& rt, sf::RenderStates) const override;

    bool active;
    sf::Vector2f basePosition;
    sf::RectangleShape hitBox;
    sf::RectangleShape topBar;
    sf::RectangleShape bodyBar;
    sf::Text* label;
};
