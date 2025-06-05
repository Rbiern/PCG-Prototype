#include "SoundBar.hpp"


SoundBar::SoundBar(unsigned int barLvl) {
    volumeLevel = barLvl;
    basePosition = sf::Vector2f{0.f, 0.f};
    // --- 2) Minus Button ------------------------------------------------
    minusBtn.setSize(sf::Vector2f{25.f, 24.f});
    minusBtn.setPosition(sf::Vector2f{0.f, 0.f});
    minusBtn.setFillColor(sf::Color{0xD9, 0xD9, 0xD9});
    // the “–” inside it
    minusBar.setSize(sf::Vector2f{17.f, 3.f});
    minusBar.setPosition(sf::Vector2f{4.f, 10.f});
    minusBar.setFillColor(sf::Color::Black);
    // --- 3) Volume Bar Background ---------------------------------------
    volBg.setSize(sf::Vector2f{179.f, 24.f});
    volBg.setPosition(sf::Vector2f{31.f, 0.f});
    volBg.setFillColor(sf::Color{0xD9, 0xD9, 0xD9});
    // prepare up to 10 black bars, equally spaced at x=37 + i*17, y=3
    for (int i = 0; i < MAX_BARS; i++) {
        bars[i].setSize(sf::Vector2f{14.f, 18.f});
        sf::Vector2f position = basePosition + sf::Vector2f(37.f + 17.f * float(i), 3.f);
        bars[i].setPosition(position);
        bars[i].setFillColor(sf::Color::Black);
    }
    // --- 4) Plus Button -------------------------------------------------
    plusBtn.setSize(sf::Vector2f{25.f, 24.f});
    plusBtn.setPosition(sf::Vector2f{216.f, 0.f});
    plusBtn.setFillColor(sf::Color{0xD9, 0xD9, 0xD9});
    // draw a “+” by two rectangles centered inside plusBtn
    // horizontal
    plusH.setSize(sf::Vector2f{17.f, 3.f});
    plusH.setPosition(sf::Vector2f{220.f, 10.f});
    plusH.setFillColor(sf::Color::Black);
    // vertical
    plusV.setSize(sf::Vector2f{3.f, 17.f});
    plusV.setPosition(sf::Vector2f{227.f, 3.f});
    plusV.setFillColor(sf::Color::Black);
}


int SoundBar::getLevel() {
    return volumeLevel;
}


bool SoundBar::handleUserInput(const sf::Event &event) {
    if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f m(mouseButtonPressed->position);
        if (minusBtn.getGlobalBounds().contains(m) && volumeLevel > 0) {
            --volumeLevel;
            return true;
        } else if (plusBtn.getGlobalBounds().contains(m) && volumeLevel < MAX_BARS) {
            ++volumeLevel;
            return true;
        }
    }
    return false;
}


void SoundBar::setPosition(sf::Vector2f position) {
    minusBtn.setPosition(position);
    minusBar.setPosition(sf::Vector2f{4.f, 10.f} + position);
    volBg.setPosition(sf::Vector2f{31.f, 0.f} + position);
    plusBtn.setPosition(sf::Vector2f{216.f, 0.f} + position);
    plusH.setPosition(sf::Vector2f{220.f, 10.f} + position);
    plusV.setPosition(sf::Vector2f{227.f, 3.f} + position);
    for (int i = 0; i < MAX_BARS; i++) {
        sf::Vector2f p = sf::Vector2f{37.f + 17.f * float(i), 3.f} + position;
        bars[i].setPosition(p);
    }
    basePosition = position;
}


void SoundBar::setScale(sf::Vector2f scale) {
    minusBtn.setSize(sf::Vector2f(25.f * scale.x, 24.f * scale.y));
    {
        sf::Vector2f newPosition = sf::Vector2f{0.f * scale.x, 0.f * scale.y} + basePosition;
        minusBtn.setPosition(newPosition);
    }
    minusBar.setSize(sf::Vector2f(17.f * scale.x, 3.f * scale.y));
    {
        sf::Vector2f newPosition = sf::Vector2f{4.f * scale.x, 10.f * scale.y} + basePosition;
        minusBar.setPosition(newPosition);
    }
    volBg.setSize(sf::Vector2f(179.f * scale.x, 24.f * scale.y));
    {
        sf::Vector2f newPosition = sf::Vector2f{31.f * scale.x, 0.f * scale.y} + basePosition;
        volBg.setPosition(newPosition);
    }
    plusBtn.setSize(sf::Vector2f(25.f * scale.x, 24.f * scale.y));
    {
        sf::Vector2f newPosition = sf::Vector2f{216.f * scale.x, 0.f * scale.y} + basePosition;
        plusBtn.setPosition(newPosition);
    }
    plusH.setSize(sf::Vector2f(17.f * scale.x, 3.f * scale.y));
    {
        sf::Vector2f newPosition = sf::Vector2f{220.f * scale.x, 10.f * scale.y} + basePosition;
        plusH.setPosition(newPosition);
    }
    plusV.setSize(sf::Vector2f(3.f * scale.x, 17.f * scale.y));
    {
        sf::Vector2f newPosition = sf::Vector2f{227.f * scale.x, 3.f * scale.y} + basePosition;
        plusV.setPosition(newPosition);
    }
    for (int i = 0; i < MAX_BARS; i++) {
        bars[i].setSize(sf::Vector2f(14.f * scale.x, 18.f * scale.y));
        sf::Vector2f p = sf::Vector2f{(37.f + 17.f * float(i)) * scale.x, 3.f * scale.y} + basePosition;
        bars[i].setPosition(p);
    }
}


void SoundBar::draw(sf::RenderTarget &rt, sf::RenderStates) const {
    rt.draw(minusBtn);
    rt.draw(minusBar);
    rt.draw(volBg);
    for (int i = 0; i < volumeLevel; i++) {
        rt.draw(bars[i]);
    }
    rt.draw(plusBtn);
    rt.draw(plusH);
    rt.draw(plusV);
}
