#include "FpsCounter.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <cmath>
#include <string>


FpsCounter::FpsCounter(const sf::Font& font)
    : text(font, "FPS: --", 24u) {

    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1.f);
}


void FpsCounter::update(float unscaledDelta) {
    if (!std::isfinite(unscaledDelta) || unscaledDelta <= 0.f) {
        return;
    }

    fpsElapsed += unscaledDelta;
    ++fpsFrames;

    if (fpsElapsed >= 0.5f) {
        const auto fps = static_cast<unsigned int>(static_cast<float>(fpsFrames) / fpsElapsed + 0.5f);

        text.setString("FPS: " + std::to_string(fps));

        fpsElapsed = 0.f;
        fpsFrames = 0;
    }
}


void FpsCounter::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    const auto size = target.getSize();
    if (size.x == 0 || size.y == 0) {
        return;
    }

    const sf::View previousView = target.getView();

    target.setView(sf::View(sf::FloatRect(
        {0.f, 0.f},
        {static_cast<float>(size.x), static_cast<float>(size.y)}
    )));

    const auto bounds = text.getLocalBounds();

    sf::Transform placement;
    placement.translate({
        static_cast<float>(size.x) - 12.f,
        12.f
    });
    placement.translate({
        -(bounds.position.x + bounds.size.x),
        -bounds.position.y
    });

    states.transform *= placement;
    target.draw(text, states);

    target.setView(previousView);
}
