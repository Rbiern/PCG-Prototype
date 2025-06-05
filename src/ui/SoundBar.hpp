#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <algorithm>


class SoundBar : public sf::Drawable {
public:
    explicit SoundBar(unsigned int barLvl);
    ~SoundBar() override = default;
    int getLevel();
    void setPosition(sf::Vector2f position);
    void setScale(sf::Vector2f scale);
    bool handleUserInput(const sf::Event &event);

private:
    void draw(sf::RenderTarget& rt, sf::RenderStates) const override;

    const int MAX_BARS = 10;
    int volumeLevel;
    sf::Vector2f basePosition;
    sf::RectangleShape minusBtn;
    sf::RectangleShape minusBar;
    sf::RectangleShape volBg;
    sf::RectangleShape plusBtn;
    sf::RectangleShape plusH;
    sf::RectangleShape plusV;
    std::array<sf::RectangleShape, 10> bars;
};
