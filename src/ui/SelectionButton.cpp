#include "SelectionButton.hpp"


SelectionButton::SelectionButton(const std::string& labelText) : active{false} {
    ResourceManager& rm = ResourceManager::getInstance();
    sf::Font& font = rm.getFont();

    hitBox.setSize(sf::Vector2f{200.f, 48.f});
    hitBox.setPosition(sf::Vector2f{0.f, 0.f});
    hitBox.setFillColor(sf::Color(0, 0, 0, 0));

    topBar.setSize(sf::Vector2f{200.f, 8.f});
    topBar.setPosition(sf::Vector2f{0.f, 0.f});
    topBar.setFillColor(sf::Color::White);

    bodyBar.setSize(sf::Vector2f{200.f, 40.f});
    bodyBar.setPosition(sf::Vector2f{0.f, 8.f});
    bodyBar.setFillColor(sf::Color(0x12, 0x94, 0xD3));

    label = new sf::Text(font, labelText, 36);
    label->setFillColor(sf::Color::White);
    {
        sf::FloatRect nb = bodyBar.getGlobalBounds();
        sf::FloatRect textBounds = label->getLocalBounds();
        float x = nb.position.x + (nb.size.x - (textBounds.size.x / 2.f) - textBounds.position.x);
        float y = nb.position.y + (nb.size.y - (textBounds.size.y / 2.f) - textBounds.position.y);
        label->setPosition(sf::Vector2f{x, y});
    }
    updateColors();
}


SelectionButton::~SelectionButton() {
    delete label;
}


void SelectionButton::updateColors() {
    if (active) {
        topBar.setFillColor(sf::Color::White);
        bodyBar.setFillColor(sf::Color(0x12, 0x94, 0xD3));
    }
    else {
        topBar.setFillColor(sf::Color(0x2D, 0x32, 0x35));
        bodyBar.setFillColor(sf::Color(0, 0, 0, 0));
    }
}


void SelectionButton::setPosition(sf::Vector2f position) {
    hitBox.setPosition(position);
    topBar.setPosition(position);
    bodyBar.setPosition(sf::Vector2f{0.f, 8.f} + position);
    {
        sf::FloatRect nb = bodyBar.getGlobalBounds();
        sf::FloatRect textBounds = label->getLocalBounds();
        float x = nb.position.x + (nb.size.x - textBounds.size.x) / 2.f - textBounds.position.x;
        float y = nb.position.y + (nb.size.y - textBounds.size.y) / 2.f - textBounds.position.y;
        label->setPosition(sf::Vector2f{x, y});
    }
    basePosition = position;
}


sf::Vector2f SelectionButton::getPosition() {
    return hitBox.getGlobalBounds().position;
}


void SelectionButton::setScale(sf::Vector2f scale) {
    sf::Vector2f newPosition;
    {
        hitBox.setSize(sf::Vector2f(200.f * scale.x, 48.f * scale.y));
        newPosition = sf::Vector2f{0.f * scale.x, 0.f * scale.y} + basePosition;
        hitBox.setPosition(newPosition);
    }
    {
        topBar.setSize(sf::Vector2f(200.f * scale.x, 8.f * scale.y));
        newPosition = sf::Vector2f{0.f * scale.x, 0.f * scale.y} + basePosition;
        topBar.setPosition(newPosition);
    }
    {
        bodyBar.setSize(sf::Vector2f(200.f * scale.x, 40.f * scale.y));
        newPosition = sf::Vector2f{0.f * scale.x, 8.f * scale.y} + basePosition;
        bodyBar.setPosition(newPosition);
    }
    label->setScale(scale);
    {
        sf::FloatRect nb = bodyBar.getGlobalBounds();
        sf::FloatRect textBounds = label->getLocalBounds();
        float x = nb.position.x + (nb.size.x - (textBounds.size.x * scale.x)) / 2.f - (textBounds.position.x * scale.x);
        float y = nb.position.y + (nb.size.y - (textBounds.size.y * scale.y)) / 2.f - (textBounds.position.y * scale.y);
        label->setPosition(sf::Vector2f{x, y});
    }
}


bool SelectionButton::contains(sf::Vector2f mousePos) const {
    return hitBox.getGlobalBounds().contains(mousePos);
}


void SelectionButton::draw(sf::RenderTarget& rt, sf::RenderStates) const {
    rt.draw(hitBox);
    rt.draw(topBar);
    rt.draw(bodyBar);
    rt.draw(*label);
}
