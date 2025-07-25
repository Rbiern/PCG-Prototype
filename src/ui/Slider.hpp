#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <algorithm>
#include "../core/ResourceManager.hpp"


class Slider : public sf::Drawable {
public:
    explicit Slider(const std::array<std::string,3>& labels);
    ~Slider()override;
    void setPosition(sf::Vector2f position);
    void setScale(sf::Vector2f scale);
    void handleEvent(const sf::Event& evt);
    int getValue() const;       // Returns 0, 1 or 2
    void setValue(int value);

private:
    void snapTo(int i);
    void draw(sf::RenderTarget& rt, sf::RenderStates) const override;

    bool m_dragging;
    int m_index;
    sf::Vector2f basePosition;
    sf::Vector2f scaling;
    sf::Vector2f m_dragOffset;
    sf::Vector2f m_barBoxOffset;
    std::array<sf::Vector2f, 3> m_boxOffsets;
    sf::RectangleShape m_track;
    sf::RectangleShape m_knobBox;
    sf::RectangleShape m_knobBar;
    std::array<sf::RectangleShape, 3> m_ticks;
    std::array<sf::Text*, 3> m_tickLabels;
};
