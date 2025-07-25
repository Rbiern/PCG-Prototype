#include "SidePanel.hpp"


SidePanel::SidePanel() {
    ResourceManager& rm = ResourceManager::getInstance();
    sf::Font& font = rm.getFont();
    isVisible = false;

    panel.setSize(sf::Vector2f{288.f, 1080.f});
    panel.setPosition(sf::Vector2f{1632.f, 0.f});
    panel.setFillColor(sf::Color(0x15, 0x12, 0x26));

    divider.setSize(sf::Vector2f{8.f, 1080.f});
    divider.setPosition(sf::Vector2f{1632.f, 0.f});
    divider.setFillColor(sf::Color(0xD8, 0xA4, 0x0F));

    pauseButtonTexture = rm.getTexture("../../images/buttons/pause_button.png");
    pauseButtonSprite = new sf::Sprite(pauseButtonTexture);
    {
        auto ts = pauseButtonSprite->getTexture().getSize();
        float sx = (48.f * 1.f) / float(ts.x);
        float sy = (48.f * 1.f) / float(ts.y);
        pauseButtonSprite->setScale(sf::Vector2f{sx, sy});
        pauseButtonSprite->setPosition(sf::Vector2f{1670.f, 13.f});
    }

    homeButtonTexture = rm.getTexture("../../images/buttons/panel_home_button.png");
    homeButtonSprite = new sf::Sprite(homeButtonTexture);
    {
        auto ts = homeButtonSprite->getTexture().getSize();
        float sx = (133.f * 1.f) / float(ts.x);
        float sy = (48.f * 1.f) / float(ts.y);
        homeButtonSprite->setScale(sf::Vector2f{sx, sy});
        homeButtonSprite->setPosition(sf::Vector2f{1748.f, 13.f});
    }

    abilitiesButtonTexture = rm.getTexture("../../images/buttons/abilities.png");
    abilitiesButtonSprite = new sf::Sprite(abilitiesButtonTexture);
    {
        auto ts = abilitiesButtonSprite->getTexture().getSize();
        float sx = (124.f * 1.f) / float(ts.x);
        float sy = (43.f * 1.f) / float(ts.y);
        abilitiesButtonSprite->setScale(sf::Vector2f{sx, sy});
        abilitiesButtonSprite->setPosition(sf::Vector2f{1714.f, 355.f});
    }

    coinsTexture = rm.getTexture("../../images/gold.png");
    coinsSprite = new sf::Sprite(coinsTexture);
    {
        sf::Vector2u size = coinsTexture.getSize();
        float scaleX = 61.f / size.x;
        float scaleY = 61.f / size.y;
        coinsSprite->setScale(sf::Vector2f{scaleX, scaleY});
    }
    coinsSprite->setPosition(sf::Vector2f{1672.f, 136.f});


    heartTexture = rm.getTexture("../../images/heart.png");
    heartSprite = new sf::Sprite(heartTexture);
    {
        sf::Vector2u size = heartTexture.getSize();
        float scaleX = 57.f / size.x;
        float scaleY = 57.f / size.y;
        heartSprite->setScale(sf::Vector2f{scaleX, scaleY});
    }
    heartSprite->setPosition(sf::Vector2f{1670.f, 195.f});


    remainingEnemiesTexture = rm.getTexture("../../images/e.png");
    remainingEnemiesSprite = new sf::Sprite(remainingEnemiesTexture);
    {
        sf::Vector2u size = remainingEnemiesTexture.getSize();
        float scaleX = 57.f / size.x;
        float scaleY = 57.f / size.y;
        remainingEnemiesSprite->setScale(sf::Vector2f{scaleX, scaleY});
    }
    remainingEnemiesSprite->setPosition(sf::Vector2f{1670.f, 268.f});


    frameTexture = rm.getTexture("../../images/elements/frame.png");
    frameSprite = new sf::Sprite(frameTexture);
    {
        auto ts = frameSprite->getTexture().getSize();
        float sx = (268.f * 1.f) / float(ts.x);
        float sy = (663.f * 1.f) / float(ts.y);
        frameSprite->setScale(sf::Vector2f{sx, sy});
        frameSprite->setPosition(sf::Vector2f{1645.f, 411.f});
    }

    roundText = new sf::Text(font, "Round ", 64);
    roundText->setFillColor(sf::Color::Black);
    roundText->setOutlineColor(sf::Color::White);
    roundText->setOutlineThickness(2.f);
    {
        sf::FloatRect textBounds = roundText->getGlobalBounds();
        float y = 83.f - textBounds.size.y / 2.f;
        roundText->setPosition(sf::Vector2f{1660.f, y});
    }

    goldText = new sf::Text(font, " Gold", 36);
    goldText->setFillColor(sf::Color(0xFF, 0xD5, 0x4B));
    {
        sf::FloatRect textBounds = goldText->getGlobalBounds();
        float y = 157.f - textBounds.size.y / 2.f;
        goldText->setPosition(sf::Vector2f{1749.f, y});
    }

    healthText = new sf::Text(font, " Health", 36);
    healthText->setFillColor(sf::Color(0xEB, 0x29, 0x2B));
    {
        sf::FloatRect textBounds = healthText->getGlobalBounds();
        float y = 211.f - textBounds.size.y / 2.f;
        healthText->setPosition(sf::Vector2f{1749.f, y});
    }

    remainingText = new sf::Text(font, " Remaining\nEnemies", 36);
    remainingText->setFillColor(sf::Color(0xD8, 0x2A, 0xE8));
    {
        sf::FloatRect textBounds = remainingText->getGlobalBounds();
        float y = 265.f - textBounds.size.y / 2.f;
        remainingText->setPosition(sf::Vector2f{1749.f, y});
    }





    towerControlPanel.setSize(sf::Vector2f{280.f, 671.f});
    towerControlPanel.setPosition(sf::Vector2f{1640.f, 409.f});
    towerControlPanel.setFillColor(sf::Color(0x21, 0x1D, 0x39));

    towerControlDivider.setSize(sf::Vector2f{280.f, 6.f});
    towerControlDivider.setPosition(sf::Vector2f{1640.f, 409.f});
    towerControlDivider.setFillColor(sf::Color(0xD8, 0xA4, 0x0F));


    xButtonTexture = rm.getTexture("../../images/buttons/x_button.png");
    xButtonSprite = new sf::Sprite(xButtonTexture);
    {
        sf::Vector2u size = xButtonTexture.getSize();
        float scaleX = 38.f / size.x;
        float scaleY = 64.f / size.y;
        xButtonSprite->setScale({scaleX, scaleY});
    }
    xButtonSprite->setPosition({1882.f, 391.f});

    towerFrameTexture = rm.getTexture("../../images/elements/tower_frame.png");
    towerFrameSprite = new sf::Sprite(towerFrameTexture);
    {
        sf::Vector2u size = towerFrameTexture.getSize();
        float scaleX = 162.f / size.x;
        float scaleY = 162.f / size.y;
        towerFrameSprite->setScale({scaleX, scaleY});
    }
    towerFrameSprite->setPosition({1697.f, 483.f});

    selectedTowerTexture = rm.getTexture("../../images/healer1.png");
    selectedTowerSprite = new sf::Sprite(selectedTowerTexture);
    {
        sf::Vector2u size = selectedTowerTexture.getSize();
        float scaleX = 150.f / size.x;
        float scaleY = 150.f / size.y;
        selectedTowerSprite->setScale({scaleX, scaleY});
    }
    selectedTowerSprite->setPosition({1703.f, 489.f});

    leftArrowButtonTexture = rm.getTexture("../../images/buttons/left_arrow_button.png");
    leftArrowButtonSprite = new sf::Sprite(leftArrowButtonTexture);
    {
        sf::Vector2u size = leftArrowButtonTexture.getSize();
        float scaleX = 25.f / size.x;
        float scaleY = 24.f / size.y;
        leftArrowButtonSprite->setScale({scaleX, scaleY});
    }
    leftArrowButtonSprite->setPosition({1676.f, 657.f});

    rightArrowButtonTexture = rm.getTexture("../../images/buttons/right_arrow_button.png");
    rightArrowButtonSprite = new sf::Sprite(rightArrowButtonTexture);
    {
        sf::Vector2u size = rightArrowButtonTexture.getSize();
        float scaleX = 25.f / size.x;
        float scaleY = 24.f / size.y;
        rightArrowButtonSprite->setScale({scaleX, scaleY});
    }
    rightArrowButtonSprite->setPosition({1853.f, 657.f});

    directionIconTexture = rm.getTexture("../../images/elements/direction.png");
    directionIconSprite = new sf::Sprite(directionIconTexture);
    {
        sf::Vector2u size = directionIconTexture.getSize();
        float scaleX = 50.f / size.x;
        float scaleY = 50.f / size.y;
        directionIconSprite->setScale({scaleX, scaleY});
    }
    directionIconSprite->setPosition({1755.f, 769.f});

    northButtonTexture = rm.getTexture("../../images/buttons/north_button.png");
    northButtonSprite = new sf::Sprite(northButtonTexture);
    {
        sf::Vector2u size = northButtonTexture.getSize();
        float scaleX = 59.f / size.x;
        float scaleY = 27.f / size.y;
        northButtonSprite->setScale({scaleX, scaleY});
    }
    northButtonSprite->setPosition({1751.f, 739.f});

    southButtonTexture = rm.getTexture("../../images/buttons/south_button.png");
    southButtonSprite = new sf::Sprite(southButtonTexture);
    {
        sf::Vector2u size = southButtonTexture.getSize();
        float scaleX = 59.f / size.x;
        float scaleY = 27.f / size.y;
        southButtonSprite->setScale({scaleX, scaleY});
    }
    southButtonSprite->setPosition({1753.f, 821.f});

    westButtonTexture = rm.getTexture("../../images/buttons/west_button.png");
    westButtonSprite = new sf::Sprite(westButtonTexture);
    {
        sf::Vector2u size = westButtonTexture.getSize();
        float scaleX = 59.f / size.x;
        float scaleY = 27.f / size.y;
        westButtonSprite->setScale({scaleX, scaleY});
    }
    westButtonSprite->setPosition({1693.f, 781.f});

    eastButtonTexture = rm.getTexture("../../images/buttons/east_button.png");
    eastButtonSprite = new sf::Sprite(eastButtonTexture);
    {
        sf::Vector2u size = eastButtonTexture.getSize();
        float scaleX = 59.f / float(size.x);
        float scaleY = 27.f / size.y;
        eastButtonSprite->setScale({scaleX, scaleY});
    }
    eastButtonSprite->setPosition({1808.f, 781.f});

    sellButtonTexture = rm.getTexture("../../images/buttons/blank_button.png");
    sellButtonSprite = new sf::Sprite(sellButtonTexture);
    {
        sf::Vector2u size = sellButtonTexture.getSize();
        float scaleX = 124.f / size.x;
        float scaleY = 42.f / size.y;
        sellButtonSprite->setScale({scaleX, scaleY});
    }
    sellButtonSprite->setPosition({1715.f, 1026.f});









    towerNameText = new sf::Text(font, "Ana Clerk", 36);
    towerNameText->setFillColor(sf::Color::White);
    {
        sf::FloatRect textBounds = towerNameText->getGlobalBounds();
        float y = 417.f - textBounds.size.y / 2.f;
        towerNameText->setPosition(sf::Vector2f{1715.f, y});
    }

    killCountText = new sf::Text(font, "Enemy Kill count : 15", 24);
    killCountText->setFillColor(sf::Color::White);
    {
        sf::FloatRect textBounds = killCountText->getGlobalBounds();
        float y = 454.f - textBounds.size.y / 2.f;
        killCountText->setPosition(sf::Vector2f{1689.f, y});
    }

    targetingText = new sf::Text(font, "Target : First", 24);
    targetingText->setFillColor(sf::Color::White);
    {
        sf::FloatRect textBounds = targetingText->getGlobalBounds();
        float y = 658.f - textBounds.size.y / 2.f;
        targetingText->setPosition(sf::Vector2f{1720.f, y});
    }

    directionText = new sf::Text(font, "Direction", 36);
    directionText->setFillColor(sf::Color::White);
    {
        sf::FloatRect textBounds = directionText->getGlobalBounds();
        float y = 697.f - textBounds.size.y / 2.f;
        directionText->setPosition(sf::Vector2f{1721.f, y});
    }

    sellTowerText = new sf::Text(font, "Sell: $??", 36);
    sellTowerText->setFillColor(sf::Color(0x22, 0x88, 0x25));
    {
        sf::FloatRect textBounds = sellTowerText->getGlobalBounds();
        float y = 1031.f - textBounds.size.y / 2.f;
        sellTowerText->setPosition(sf::Vector2f{1720.f, y});
    }


}


SidePanel::~SidePanel() {
    delete pauseButtonSprite;
    delete homeButtonSprite;
    delete abilitiesButtonSprite;
    delete coinsSprite;
    delete heartSprite;
    delete remainingEnemiesSprite;
    delete frameSprite;
    delete xButtonSprite;
    delete towerFrameSprite;
    delete selectedTowerSprite;
    delete leftArrowButtonSprite;
    delete rightArrowButtonSprite;
    delete directionIconSprite;
    delete northButtonSprite;
    delete southButtonSprite;
    delete westButtonSprite;
    delete eastButtonSprite;
    delete sellButtonSprite;
    delete roundText;
    delete goldText;
    delete healthText;
    delete remainingText;
    delete towerNameText;
    delete killCountText;
    delete targetingText;
    delete directionText;
    delete sellTowerText;
}


void SidePanel::setScale(sf::Vector2f scale) {
    sf::Vector2f newPosition;

    panel.setSize(sf::Vector2f{288.f * scale.x, 1080.f * scale.y});
    newPosition = sf::Vector2f{1632.f * scale.x, 0.f * scale.y} + basePosition;
    panel.setPosition(newPosition);

    divider.setSize(sf::Vector2f{8.f * scale.x, 1080.f * scale.y});
    newPosition = sf::Vector2f{1632.f * scale.x, 0.f * scale.y} + basePosition;
    divider.setPosition(newPosition);

    towerControlPanel.setSize(sf::Vector2f{280.f * scale.x, 671.f * scale.y});
    newPosition = sf::Vector2f{1640.f * scale.x, 409.f * scale.y};
    towerControlPanel.setPosition(newPosition);

    towerControlDivider.setSize(sf::Vector2f{280.f * scale.x, 6.f * scale.y});
    newPosition = sf::Vector2f{1640.f * scale.x, 409.f * scale.y};
    towerControlDivider.setPosition(newPosition);





    {
        auto ts = pauseButtonSprite->getTexture().getSize();
        float sx = (48.f * scale.x) / float(ts.x);
        float sy = (48.f * scale.y) / float(ts.y);
        pauseButtonSprite->setScale(sf::Vector2f{sx, sy});
        pauseButtonSprite->setPosition(sf::Vector2f{1670.f * scale.x, 13.f * scale.y});
    }

    {
        auto ts = homeButtonSprite->getTexture().getSize();
        float sx = (133.f * scale.x) / float(ts.x);
        float sy = (48.f * scale.y) / float(ts.y);
        homeButtonSprite->setScale(sf::Vector2f{sx, sy});
        homeButtonSprite->setPosition(sf::Vector2f{1748.f * scale.x, 13.f * scale.y});
    }

    {
        auto ts = abilitiesButtonSprite->getTexture().getSize();
        float sx = (124.f * scale.x) / float(ts.x);
        float sy = (43.f * scale.y) / float(ts.y);
        abilitiesButtonSprite->setScale(sf::Vector2f{sx, sy});
        abilitiesButtonSprite->setPosition(sf::Vector2f{1714.f * scale.x, 355.f * scale.y});
    }



    {
        sf::Vector2u size = coinsTexture.getSize();
        float scaleX = (61.f * scale.x) / size.x;
        float scaleY = (61.f * scale.y) / size.y;
        coinsSprite->setScale({scaleX, scaleY});
    }
    coinsSprite->setPosition({1672.f * scale.x, 136.f * scale.y});

    {
        sf::Vector2u size = heartTexture.getSize();
        float scaleX = (57.f * scale.x) / size.x;
        float scaleY = (57.f * scale.y) / size.y;
        heartSprite->setScale({scaleX, scaleY});
    }
    heartSprite->setPosition({1670.f * scale.x, 195.f * scale.y});

    {
        sf::Vector2u size = remainingEnemiesTexture.getSize();
        float scaleX = (57.f * scale.x) / size.x;
        float scaleY = (57.f * scale.y) / size.y;
        remainingEnemiesSprite->setScale({scaleX, scaleY});
    }
    remainingEnemiesSprite->setPosition({1670.f * scale.x, 268.f * scale.y});


    {
        auto ts = frameSprite->getTexture().getSize();
        float sx = (268.f * scale.x) / float(ts.x);
        float sy = (663.f * scale.y) / float(ts.y);
        newPosition = sf::Vector2f{1645.f * scale.x, 411.f * scale.y};
        frameSprite->setScale(sf::Vector2f{sx, sy});
        frameSprite->setPosition(newPosition);
    }
//*************************************************************************


    {
        sf::Vector2u ts = xButtonTexture.getSize();
        float sx = (38.f * scale.x) / float(ts.x);
        float sy = (64.f * scale.y) / float(ts.y);
        xButtonSprite->setScale(sf::Vector2f{sx, sy});
        xButtonSprite->setPosition(sf::Vector2f{1882.f * scale.x, 391.f * scale.y});
    }
    {
        sf::Vector2u ts = towerFrameTexture.getSize();
        float sx = (162.f * scale.x) / float(ts.x);
        float sy = (162.f * scale.y) / float(ts.y);
        towerFrameSprite->setScale(sf::Vector2f{sx, sy});
        towerFrameSprite->setPosition(sf::Vector2f{1697.f * scale.x, 483.f * scale.y});
    }
    {
        sf::Vector2u ts = selectedTowerTexture.getSize();
        float sx = (150.f * scale.x) / float(ts.x);
        float sy = (150.f * scale.y) / float(ts.y);
        selectedTowerSprite->setScale(sf::Vector2f{sx, sy});
        selectedTowerSprite->setPosition(sf::Vector2f{1703.f * scale.x, 489.f * scale.y});
    }

    {
        sf::Vector2u ts = leftArrowButtonTexture.getSize();
        float sx = (25.f * scale.x) / float(ts.x);
        float sy = (24.f * scale.y) / float(ts.y);
        leftArrowButtonSprite->setScale(sf::Vector2f{sx, sy});
        leftArrowButtonSprite->setPosition(sf::Vector2f{1676.f * scale.x, 657.f * scale.y});
    }
    {
        sf::Vector2u ts = rightArrowButtonTexture.getSize();
        float sx = (25.f * scale.x) / float(ts.x);
        float sy = (24.f * scale.y) / float(ts.y);
        rightArrowButtonSprite->setScale(sf::Vector2f{sx, sy});
        rightArrowButtonSprite->setPosition(sf::Vector2f{1853.f * scale.x, 657.f * scale.y});
    }
    {
        sf::Vector2u ts = directionIconTexture.getSize();
        float sx = (50.f * scale.x) / float(ts.x);
        float sy = (50.f * scale.y) / float(ts.y);
        directionIconSprite->setScale(sf::Vector2f{sx, sy});
        directionIconSprite->setPosition(sf::Vector2f{1755.f * scale.x, 769.f * scale.y});
    }
    {
        sf::Vector2u ts = northButtonTexture.getSize();
        float sx = (59.f * scale.x) / float(ts.x);
        float sy = (27.f * scale.y) / float(ts.y);
        northButtonSprite->setScale(sf::Vector2f{sx, sy});
        northButtonSprite->setPosition(sf::Vector2f{1751.f * scale.x, 737.f * scale.y});
    }
    {
        sf::Vector2u ts = southButtonTexture.getSize();
        float sx = (59.f * scale.x) / float(ts.x);
        float sy = (27.f * scale.y) / float(ts.y);
        southButtonSprite->setScale(sf::Vector2f{sx, sy});
        southButtonSprite->setPosition(sf::Vector2f{1753.f * scale.x, 823.f * scale.y});
    }
    {
        sf::Vector2u ts = westButtonTexture.getSize();
        float sx = (59.f * scale.x) / float(ts.x);
        float sy = (27.f * scale.y) / float(ts.y);
        westButtonSprite->setScale(sf::Vector2f{sx, sy});
        westButtonSprite->setPosition(sf::Vector2f{1691.f * scale.x, 781.f * scale.y});
    }
    {
        sf::Vector2u ts = eastButtonTexture.getSize();
        float sx = (59.f * scale.x) / float(ts.x);
        float sy = (27.f * scale.y) / float(ts.y);
        eastButtonSprite->setScale(sf::Vector2f{sx, sy});
    }
    eastButtonSprite->setPosition(sf::Vector2f{1810.f * scale.x, 781.f * scale.y});
    {
        sf::Vector2u ts = sellButtonTexture.getSize();
        float sx = (124.f * scale.x) / float(ts.x);
        float sy = (42.f * scale.y) / float(ts.y);
        sellButtonSprite->setScale(sf::Vector2f{sx, sy});
    }
    sellButtonSprite->setPosition(sf::Vector2f{1715.f * scale.x, 1026.f * scale.y});


    roundText->setScale(scale);
    {
        sf::FloatRect ts = roundText->getLocalBounds();
        float x = 1660.f * scale.x;
        float y = (83.f * scale.y) - ts.size.y / 2.f;
        roundText->setPosition(sf::Vector2f{x, y});
    }
    goldText->setScale(scale);
    {
        sf::FloatRect ts = goldText->getLocalBounds();
        float x = 1749.f * scale.x;
        float y = (157.f * scale.y) - ts.size.y / 2.f;
        goldText->setPosition(sf::Vector2f{x, y});
    }
    healthText->setScale(scale);
    {
        sf::FloatRect ts = healthText->getLocalBounds();
        float x = 1749.f * scale.x;
        float y = (211.f * scale.y) - ts.size.y / 2.f;
        healthText->setPosition(sf::Vector2f{x, y});
    }
    remainingText->setScale(scale);
    {
        float x = 1749.f * scale.x;
        float y = (260.f * scale.y);
        remainingText->setPosition(sf::Vector2f{x, y});
    }
    towerNameText->setScale(scale);
    {
        sf::FloatRect ts = towerNameText->getLocalBounds();
        float x = 1715.f * scale.x;
        float y = 417.f * scale.y - ts.size.y / 2.f;
        towerNameText->setPosition(sf::Vector2f{x, y});
    }
    killCountText->setScale(scale);
    {
        sf::FloatRect ts = killCountText->getLocalBounds();
        float x = 1689.f * scale.x;
        float y = 454.f * scale.y - ts.size.y / 2.f;
        killCountText->setPosition(sf::Vector2f{x, y});
    }
    targetingText->setScale(scale);
    {
        sf::FloatRect ts = targetingText->getLocalBounds();
        float x = 1720.f * scale.x;
        float y = (658.f * scale.y) - ts.size.y / 4.f;
        targetingText->setPosition(sf::Vector2f{x, y});
    }
    directionText->setScale(scale);
    {
        sf::FloatRect ts = directionText->getLocalBounds();
        float x = 1721.f * scale.x;
        float y = (697.f * scale.y) - ts.size.y / 2.f;
        directionText->setPosition(sf::Vector2f{x, y});
    }
    sellTowerText->setScale(scale);
    {
        sf::FloatRect ts = sellTowerText->getLocalBounds();
        float x = 1720.f * scale.x;
        float y = (1031.f * scale.y) - ts.size.y / 2.f;
        sellTowerText->setPosition(sf::Vector2f{x, y});
    }
}


void SidePanel::setRoundNumber(int stage) {
    roundText->setString("Round " + std::to_string(stage));
}


void SidePanel::setGoldNumber(int gold) {
    goldText->setString(std::to_string(gold) + " Gold");
}


void SidePanel::setHealthNumber(int health) {
   healthText->setString(std::to_string(health) + " Health");
}


void SidePanel::setRemainingNumber(int count) {
    remainingText->setString(std::to_string(count) + " Remaining\nEnemies");
}


void SidePanel::draw(sf::RenderTarget &rt, sf::RenderStates) const {
    rt.draw(panel);
    rt.draw(divider);
    rt.draw(*pauseButtonSprite);
    rt.draw(*homeButtonSprite);
    rt.draw(*abilitiesButtonSprite);
    rt.draw(*coinsSprite);
    rt.draw(*heartSprite);
    rt.draw(*remainingEnemiesSprite);
    rt.draw(*roundText);
    rt.draw(*goldText);
    rt.draw(*healthText);
    rt.draw(*remainingText);
    if (isVisible) {
        rt.draw(towerControlPanel);
        rt.draw(towerControlDivider);
        rt.draw(*xButtonSprite);
        rt.draw(*towerFrameSprite);
        rt.draw(*selectedTowerSprite);
        rt.draw(*leftArrowButtonSprite);
        rt.draw(*rightArrowButtonSprite);
        rt.draw(*directionIconSprite);
        rt.draw(*northButtonSprite);
        rt.draw(*southButtonSprite);
        rt.draw(*westButtonSprite);
        rt.draw(*eastButtonSprite);
        rt.draw(*sellButtonSprite);
        rt.draw(*towerNameText);
        rt.draw(*killCountText);
        rt.draw(*targetingText);
        rt.draw(*directionText);
        rt.draw(*sellTowerText);
    }
    else {
        rt.draw(*frameSprite);
    }
}
