#include "PlayerCard.hpp"


PlayerCard::PlayerCard(const std::string& id) {
    ResourceManager& rm = ResourceManager::getInstance();
    sf::Font& font = rm.getFont();
    towerData = rm.getTowerData(id);
    basePosition = sf::Vector2f{0.f, 0.f};
    sf::Color color;

    const static std::map<std::string, sf::Color> CLASS_COLORS {
            {"Support", sf::Color{0x12, 0x94, 0xD3}},
            {"Range",   sf::Color{0xFF, 0x00, 0x00}},
            {"Defense", sf::Color{0x20, 0x9F, 0x37}},
            {"Melee", sf::Color{0xF1, 0xCE, 0x00}}
    };
    auto it = CLASS_COLORS.find(towerData.towerClass);
    color = (it != CLASS_COLORS.end() ? it->second : sf::Color{0x62, 0x6D, 0x64});

    hitBox.setSize(sf::Vector2f{120.f, 154.f});
    hitBox.setPosition(sf::Vector2f{0.f, 0.f});
    hitBox.setFillColor(sf::Color(0.f, 0.f, 0.f, 0.f));

    overlay.setSize(sf::Vector2f{120.f, 150.f});
    overlay.setPosition(sf::Vector2f{0.f, 2.f});
    overlay.setFillColor(sf::Color(0x3C, 0x3F, 0x45, 128));

    topBar.setSize(sf::Vector2f{120.f, 2.f});
    topBar.setPosition(sf::Vector2f{0.f, 0.f});
    topBar.setFillColor(sf::Color(217, 217, 217));

    bottomBar.setSize(sf::Vector2f{120.f, 2.f});
    bottomBar.setPosition(sf::Vector2f{0.f, 152.f});
    bottomBar.setFillColor(sf::Color(217, 217, 217));

    classTopBar.setSize(sf::Vector2f{60.f, 2.f});
    classTopBar.setPosition(sf::Vector2f{30.f, 0.f});
    classTopBar.setFillColor(color);

    classBottomBar.setSize(sf::Vector2f{60.f, 2.f});
    classBottomBar.setPosition(sf::Vector2f{30.f, 152.f});
    classBottomBar.setFillColor(color);

    iconBackground.setSize(sf::Vector2f{30.f, 30.f});
    iconBackground.setPosition(sf::Vector2f{30.f, 2.f});
    iconBackground.setFillColor(sf::Color::White);

    costBackground.setSize(sf::Vector2f{30.f, 30.f});
    costBackground.setPosition(sf::Vector2f{60.f, 2.f});
    costBackground.setFillColor(sf::Color::Black);

    classIconTexture = rm.getTexture(towerData.classIconImagePath);
    classIconSprite = new sf::Sprite(classIconTexture);
    {
        auto ts = classIconSprite->getTexture().getSize();
        float sx = (30.f * 1.f) / float(ts.x);
        float sy = (30.f * 1.f) / float(ts.y);
        classIconSprite->setScale(sf::Vector2f{sx, sy});
        classIconSprite->setPosition(iconBackground.getPosition());
    }
    towerTexture = rm.getTexture(towerData.towerImagePath_a);
    towerSprite = new sf::Sprite(towerTexture);
    {
        auto ts = towerSprite->getTexture().getSize();
        float sx = (120.f * 1.f) / float(ts.x);
        float sy = (120.f * 1.f) / float(ts.y);
        towerSprite->setScale(sf::Vector2f{sx, sy});
        towerSprite->setPosition(sf::Vector2f{0, 32});
    }
    costText = new sf::Text(font, std::to_string(towerData.cost), 36);
    costText->setFillColor(sf::Color::White);
    {
        sf::FloatRect nb = costBackground.getGlobalBounds();
        sf::FloatRect textBounds = costText->getLocalBounds();
        float x = nb.position.x + (nb.size.x - textBounds.size.x) / 2.f - textBounds.position.x;
        float y = nb.position.y + (nb.size.y - textBounds.size.y) / 2.f - textBounds.position.y;
        costText->setPosition(sf::Vector2f{x, y});
    }
}


PlayerCard::~PlayerCard() {
    delete costText;
    delete classIconSprite;
    delete towerSprite;
}


void PlayerCard::setPosition(sf::Vector2f position) {
    hitBox.setPosition(position);
    overlay.setPosition(sf::Vector2f{0.f, 2.f} + position);
    topBar.setPosition(position);
    bottomBar.setPosition(sf::Vector2f{0.f, 152.f} + position);
    classTopBar.setPosition(sf::Vector2f{30.f, 0.f} + position);
    classBottomBar.setPosition(sf::Vector2f{30.f, 152.f} + position);
    iconBackground.setPosition(sf::Vector2f{30.f, 2.f} + position);
    costBackground.setPosition(sf::Vector2f{60.f, 2.f} + position);
    classIconSprite->setPosition(sf::Vector2f{30.f, 2.f} + position);
    towerSprite->setPosition(sf::Vector2f{0.f, 32.f} + position);
    {
        sf::FloatRect nb = costBackground.getGlobalBounds();
        sf::FloatRect textBounds = costText->getLocalBounds();
        float x = nb.position.x + (nb.size.x - textBounds.size.x) / 2.f - textBounds.position.x;
        float y = nb.position.y + (nb.size.y - textBounds.size.y) / 2.f - textBounds.position.y;
        costText->setPosition(sf::Vector2f{x, y});
    }
    basePosition = position;
}


sf::Vector2f PlayerCard::getPosition() {
    return hitBox.getGlobalBounds().position;
}


void PlayerCard::setScale(sf::Vector2f scale) {
    sf::Vector2f newPosition;
    {
        hitBox.setSize(sf::Vector2f(120.f * scale.x, 154.f * scale.y));
        newPosition = sf::Vector2f{0.f * scale.x, 0.f * scale.y} + basePosition;
        hitBox.setPosition(newPosition);
    }
    {
        overlay.setSize(sf::Vector2f(120.f * scale.x, 150.f * scale.y));
        newPosition = sf::Vector2f{0.f * scale.x, 2.f * scale.y} + basePosition;
        overlay.setPosition(newPosition);
    }
    {
        topBar.setSize(sf::Vector2f(120.f * scale.x, 2.f * scale.y));
        newPosition = sf::Vector2f{0.f * scale.x, 0.f * scale.y} + basePosition;
        topBar.setPosition(newPosition);
    }
    {
        bottomBar.setSize(sf::Vector2f(120.f * scale.x, 2.f * scale.y));
        newPosition = sf::Vector2f{0.f * scale.x, 152.f * scale.y} + basePosition;
        bottomBar.setPosition(newPosition);
    }
    {
        classTopBar.setSize(sf::Vector2f(60.f * scale.x, 2.f * scale.y));
        newPosition = sf::Vector2f{30.f * scale.x, 0.f * scale.y} + basePosition;
        classTopBar.setPosition(newPosition);
    }
    {
        classBottomBar.setSize(sf::Vector2f(60.f * scale.x, 2.f * scale.y));
        newPosition = sf::Vector2f{30.f * scale.x, 152.f * scale.y} + basePosition;
        classBottomBar.setPosition(newPosition);
    }
    {
        iconBackground.setSize(sf::Vector2f{30.f * scale.x, 30.f * scale.y});
        newPosition = sf::Vector2f{30.f * scale.x, 2.f * scale.y} + basePosition;
        iconBackground.setPosition(newPosition);
    }
    {
        costBackground.setSize(sf::Vector2f{30.f * scale.x, 30.f * scale.y});
        newPosition = sf::Vector2f{60.f * scale.x, 2.f * scale.y} + basePosition;
        costBackground.setPosition(newPosition);
    }
    {
        auto ts = classIconSprite->getTexture().getSize();
        float sx = (30.f * scale.x) / float(ts.x);
        float sy = (30.f * scale.y) / float(ts.y);
        newPosition = sf::Vector2f{30.f * scale.x, 2.f * scale.y} + basePosition;
        classIconSprite->setScale(sf::Vector2f{sx, sy});
        classIconSprite->setPosition(newPosition);
    }
    {
        auto ts = towerSprite->getTexture().getSize();
        float sx = (120.f * scale.x) / float(ts.x);
        float sy = (120.f * scale.y) / float(ts.y);
        newPosition = sf::Vector2f{0.f * scale.x, 32.f * scale.y} + basePosition;
        towerSprite->setScale(sf::Vector2f{sx, sy});
        towerSprite->setPosition(newPosition);
    }
    costText->setScale(scale);
    {
        sf::FloatRect nb = costBackground.getGlobalBounds();
        sf::FloatRect textBounds = costText->getLocalBounds();
        float x = nb.position.x + (nb.size.x - (textBounds.size.x * scale.x)) / 2.f - (textBounds.position.x * scale.x);
        float y = nb.position.y + (nb.size.y - (textBounds.size.y * scale.y)) / 2.f - (textBounds.position.y * scale.y);
        costText->setPosition(sf::Vector2f{x, y});
    }
}


bool PlayerCard::contains(sf::Vector2f mousePos) const {
    return hitBox.getGlobalBounds().contains(mousePos);
}


void PlayerCard::draw(sf::RenderTarget &rt, sf::RenderStates) const {
    rt.draw(hitBox);
    rt.draw(overlay);
    rt.draw(topBar);
    rt.draw(bottomBar);
    rt.draw(classTopBar);
    rt.draw(classBottomBar);
    rt.draw(iconBackground);
    rt.draw(costBackground);
    rt.draw(*costText);
    rt.draw(*towerSprite);
    rt.draw(*classIconSprite);
}
