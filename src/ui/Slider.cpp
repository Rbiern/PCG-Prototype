#include "Slider.hpp"


Slider::Slider(const std::array<std::string,3>& labels) {
    m_dragging = false;
    m_index = 0;
    ResourceManager& rm = ResourceManager::getInstance();
    sf::Font& font = rm.getFont();
    basePosition = sf::Vector2f{0.f, 0.f};
    scaling = sf::Vector2f{1.f, 1.f};

    // —— Track ——
    m_track.setSize(sf::Vector2f{236.f, 5.f});
    m_track.setPosition(sf::Vector2f{0.f, 16.f});
    m_track.setFillColor(sf::Color(0xD9, 0xD9, 0xD9));

    // —— Ticks ——
    const std::array<float, 3> tickXs = {0.f, 115.f, 231.f};
    for (int i = 0; i < 3; i++) {
        m_ticks[i].setSize(sf::Vector2f{6.f, 11.f});
        m_ticks[i].setPosition(sf::Vector2f{tickXs[i], 13.f});
        m_ticks[i].setFillColor(sf::Color(0xD9, 0xD9, 0xD9));

        // —— Label above each tick ——
        m_tickLabels[i] = new sf::Text(font, labels[i], 24);
        m_tickLabels[i]->setFillColor(sf::Color::Black);

        auto tb = m_tickLabels[i]->getLocalBounds();
        float x = tickXs[i] + 3.f - (tb.position.x + tb.size.x / 2.f);
        float y = 12.f + (tb.position.y + tb.size.y);
        m_tickLabels[i]->setPosition({x, y});
    }

    // —— Knob shapes ——
    m_knobBox.setSize(sf::Vector2f{15.f, 17.f});
    m_knobBox.setPosition(sf::Vector2f{0.f, 10.f});
    m_knobBox.setFillColor(sf::Color::Red);

    m_knobBar.setSize(sf::Vector2f{5.f, 37.f});
    m_knobBar.setPosition(sf::Vector2f{5.f, 0.f});
    m_knobBar.setFillColor(sf::Color::Red);

    // Discrete slot offsets for the box (x, y)
    m_boxOffsets = {{ {0.f, 10.f}, {111.f, 10.f}, {222.f, 10.f} }};
    // constant offset from box to bar
    m_barBoxOffset = sf::Vector2f{5.f, -10.f};
}


Slider::~Slider() {
    for (auto txt : m_tickLabels) {
        delete txt;
    }
}


void Slider::setPosition(sf::Vector2f position) {
    m_track.setPosition(sf::Vector2f{0.f, 16.f} + position);
    const std::array<float, 3> tickXs = {0.f, 115.f, 231.f};
    for (int i = 0; i < 3; i++) {
        m_ticks[i].setPosition(sf::Vector2f{tickXs[i], 13.f} + position);
        sf::FloatRect tb = m_tickLabels[i]->getLocalBounds();
        float y = 10.f - (tb.position.y + tb.size.y);
        float x = tickXs[i] + 3.f - (tb.position.x + tb.size.x / 2.f) - 20.f;
        m_tickLabels[i]->setPosition(sf::Vector2f{x, y} + position);
    }
    m_tickLabels[0]->setPosition({m_tickLabels[0]->getPosition().x + 5, m_tickLabels[0]->getPosition().y});
    m_tickLabels[2]->setPosition({m_tickLabels[2]->getPosition().x - 5, m_tickLabels[2]->getPosition().y});

    m_knobBox.setPosition(sf::Vector2f{0.f, 10.f} + position);
    m_knobBar.setPosition(sf::Vector2f{5.f, 0.f} + position);
    basePosition = position;
}


void Slider::setScale(sf::Vector2f scale) {
    sf::Vector2f newPosition;
    {
        m_track.setSize(sf::Vector2f(236.f * scale.x, 5.f * scale.y));
        newPosition = sf::Vector2f{0.f * scale.x, 16.f * scale.y} + basePosition;
        m_track.setPosition(newPosition);
    }

    const std::array<float, 3> tick = {0.f, 115.f, 231.f};
    for (int i = 0; i < 3; i++) {
        {
            m_ticks[i].setSize(sf::Vector2f(6.f * scale.x, 11.f * scale.y));
            newPosition = sf::Vector2f{tick[i] * scale.x, 13.f * scale.y} + basePosition;
            m_ticks[i].setPosition(newPosition);
        }
        m_tickLabels[i]->setScale(scale);
        {
            sf::FloatRect tb = m_tickLabels[i]->getLocalBounds();
            float x = (tick[i] + 3.f - (tb.position.x + tb.size.x / 2.f)) * scale.y + basePosition.x;
            float y = ((10.f - (tb.position.y + tb.size.y)) - 20.f) * scale.x + basePosition.y;
            m_tickLabels[i]->setPosition(sf::Vector2f{x, y});
        }
    }


    {
        m_knobBox.setSize(sf::Vector2f(15.f * scale.x, 17.f * scale.y));
        newPosition = sf::Vector2f{0.f * scale.x, 10.f * scale.y} + basePosition;
        m_knobBox.setPosition(newPosition);
    }
    {
        m_knobBar.setSize(sf::Vector2f(5.f * scale.x, 37.f * scale.y));
        newPosition = sf::Vector2f{5.f * scale.x, 0.f * scale.y} + basePosition;
        m_knobBar.setPosition(newPosition);
    }
    scaling = scale;
}


void Slider::handleEvent(const sf::Event& evt) {
    // Convert a MouseButtonPressed into screen‐space mousePos:
    if (const auto* mbp = evt.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mousePos{ float(mbp->position.x), float(mbp->position.y) };
        // test hits in screen space—no change needed here
        if (m_knobBox.getGlobalBounds().contains(mousePos)
            || m_knobBar.getGlobalBounds().contains(mousePos))
        {
            m_dragging   = true;
            m_dragOffset = mousePos - m_knobBox.getPosition();
        }
    } else if (evt.getIf<sf::Event::MouseButtonReleased>()) {
        if (m_dragging) {
            m_dragging = false;
            // Snap to nearest slot in *screen* coords
            float localX = m_knobBox.getPosition().x - basePosition.x;
            int bestI = 0;
            float bestD = std::abs(localX - m_boxOffsets[0].x * scaling.x);
            for (int i = 1; i < 3; i++) {
                float targetX = m_boxOffsets[i].x * scaling.x;
                float d = std::abs(localX - targetX);
                if (d < bestD) {
                    bestD = d;
                    bestI = i;
                }
            }
            snapTo(bestI);
        }
    } else if (m_dragging) {
        if (const auto* mm = evt.getIf<sf::Event::MouseMoved>()) {
            sf::Vector2f mousePos{ float(mm->position.x), float(mm->position.y) };
            // raw new X in screen space
            float x = mousePos.x - m_dragOffset.x;

            // clamp to scaled slot range
            float minX = basePosition.x + m_boxOffsets[0].x * scaling.x;
            float maxX = basePosition.x + m_boxOffsets[2].x * scaling.x;
            x = std::clamp(x, minX, maxX);

            // position knobBox & knobBar
            m_knobBox.setPosition({ x,
                                    basePosition.y + m_boxOffsets[0].y * scaling.y });
            m_knobBar.setPosition({
                                          x + m_barBoxOffset.x * scaling.x,
                                          basePosition.y
                                  });
        }
    }
}


int Slider::getValue() const {
    return m_index;
}


void Slider::snapTo(int i) {
    m_index = i;
    // compute the *screen* position of the i’th slot:
    sf::Vector2f newBoxPos {
            basePosition.x + m_boxOffsets[i].x * scaling.x,
            basePosition.y + m_boxOffsets[i].y * scaling.y
    };
    m_knobBox.setPosition(newBoxPos);
    m_knobBar.setPosition({
                                  newBoxPos.x + m_barBoxOffset.x * scaling.x,
                                  newBoxPos.y + m_barBoxOffset.y * scaling.y
                          });
}


void Slider::draw(sf::RenderTarget& rt, sf::RenderStates) const {
    rt.draw(m_track);
    for (const auto& t : m_ticks) {
        rt.draw(t);
    }
    rt.draw(m_knobBar);
    rt.draw(m_knobBox);
    for (auto& lbl : m_tickLabels) {
        rt.draw(*lbl);
    }
}

void Slider::setValue(int value) {
    snapTo(value);
}
