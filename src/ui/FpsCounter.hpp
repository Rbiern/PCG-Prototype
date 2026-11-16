#pragma once

#include <SFML/Graphics/Text.hpp>


namespace sf {
    class Font;
    class RenderWindow;
}

class FpsCounter : public sf::Drawable {
public:
    explicit FpsCounter(const sf::Font& font);
    void update(float unscaledDelta);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Text text;
    float fpsElapsed = 0.f;
    unsigned int fpsFrames = 0u;
};
