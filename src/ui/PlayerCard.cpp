#include "PlayerCard.hpp"


PlayerCard::PlayerCard(float baseX, float baseY, const std::string& id) : rm(ResourceManager::getInstance()) {
    TowerData towerData = rm.getTowerData(id);
    towerId = id;
    int cost = towerData.cost;
    imagePath = towerData.imagePath_a;

    font = rm.getFont();
    sf::Color blueColor(0x12, 0x94, 0xD3);  // #1294D3
    sf::Color greenColor(0x20, 0x9F, 0x37); // #209F37
    sf::Color yellowColor(0xF1, 0xCE, 0x00); // #F1CE00
    sf::Color redColor(0xFF, 0x00, 0x00);   // #FF0000
    sf::Color color;

    if ("Support" == towerData.towerClass) color = blueColor;
    else if ("Range" == towerData.towerClass) color = redColor;
    else if ("Defense" == towerData.towerClass) color = greenColor;
    else color = yellowColor;

    towerClass = rm.getTexture(towerData.classImage);
    tower = rm.getTexture(towerData.imagePath_a);

    // transparent background
    backgroundRect.setSize(sf::Vector2f(82.f, 85.f));
    backgroundRect.setFillColor(sf::Color(60, 63, 69, 128));
    backgroundRect.setPosition({baseX, baseY});

    // gray top bar
    topLine.setSize(sf::Vector2f(82.f, 2.f));
    topLine.setFillColor(sf::Color(217, 217, 217));
    topLine.setPosition({baseX, baseY});

    // centered red bar
    redLine.setSize(sf::Vector2f(47.f, 2.f));
    redLine.setFillColor(color);
    // Center it horizontally inside topLine
    float offsetX = (82.f - 47.f) / 2.f;
    redLine.setPosition({baseX + offsetX, baseY});

    // white box
    whiteBox1.setSize(sf::Vector2f(23.f, 20.f));
    whiteBox1.setFillColor(sf::Color::White);
    whiteBox1.setPosition({baseX + offsetX, baseY + 2.f});

    // second white box
    whiteBox2.setSize(sf::Vector2f(23.f, 20.f));
    whiteBox2.setFillColor(sf::Color::White);
    whiteBox2.setPosition({baseX + offsetX, baseY + 2.f});

    // black box
    blackBox.setSize(sf::Vector2f(23.f, 20.f));
    blackBox.setFillColor(sf::Color::Black);
    blackBox.setPosition({baseX + offsetX + 23.f, baseY + 2.f});

    // transparent shape with tower image
    towerShape.setSize(sf::Vector2f(82.f, 65.f));
    towerShape.setPosition({baseX, baseY + 22.f});

    whiteBox2.setTexture(&towerClass);
    towerShape.setTexture(&tower);

    costText = new sf::Text(font, std::to_string(cost), 24);
    costText->setFillColor(sf::Color::White);
    costText->setPosition({1110.f + 40.f + 10.f, 122.f});

    // center the text within blackBox
    sf::FloatRect textRect = costText->getLocalBounds();
    costText->setOrigin({textRect.position.x + textRect.size.x / 2.f, textRect.position.y  + textRect.size.y / 2.f});
    costText->setPosition({blackBox.getPosition().x + blackBox.getSize().x / 2.f, blackBox.getPosition().y + blackBox.getSize().y / 2.f});
}


PlayerCard::~PlayerCard() {
    delete costText;
}


void PlayerCard::drawPlayerCard(sf::RenderWindow& window) {
    window.draw(backgroundRect);
    window.draw(topLine);
    window.draw(redLine);
    window.draw(whiteBox1);
    window.draw(whiteBox2);
    window.draw(blackBox);
    window.draw(*costText);
    window.draw(towerShape);
}
