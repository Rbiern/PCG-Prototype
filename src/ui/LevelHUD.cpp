#include "LevelHUD.hpp"


LevelHUD::LevelHUD() : rm(ResourceManager::getInstance()) {
    pauseButton = rm.getTexture("../../images/pause.png");
    popUp = rm.getTexture("../../images/popup.png");
    yesButton = rm.getTexture("../../images/yes.png");
    noButton = rm.getTexture("../../images/no.png");

    pauseButtonSprite = new sf::Sprite(pauseButton);
    {
        sf::Vector2u size = pauseButton.getSize();
        float scaleX = 50.f / size.x;
        float scaleY = 50.f / size.y;
        pauseButtonSprite->setScale({scaleX, scaleY});
    }
    pauseButtonSprite->setPosition({25.f, 25.f});

    popUpSprite = new sf::Sprite(popUp);
    {
        sf::Vector2u size = popUp.getSize();
        float scaleX = 250.f / size.x;
        float scaleY = 150.f / size.y;
        popUpSprite->setScale({scaleX, scaleY});
    }
    popUpSprite->setPosition({515, 285});

    yesButtonSprite = new sf::Sprite(yesButton);
    {
        sf::Vector2u size = yesButton.getSize();
        float scaleX = 46.f / size.x;
        float scaleY = 46.f / size.y;
        yesButtonSprite->setScale({scaleX, scaleY});
    }
    yesButtonSprite->setPosition({popUpSprite->getPosition().x + 56.f, popUpSprite->getPosition().y + 66.f});

    noButtonSprite = new sf::Sprite(noButton);
    {
        sf::Vector2u size = noButton.getSize();
        float scaleX = 46.f / size.x;
        float scaleY = 46.f / size.y;
        noButtonSprite->setScale({scaleX, scaleY});
    }
    noButtonSprite->setPosition({popUpSprite->getPosition().x + 151.f, popUpSprite->getPosition().y + 66.f});

    overlay.setSize({1280.f, 720.f});
    overlay.setPosition({0.f, 0.f});
    overlay.setFillColor(sf::Color(217, 217, 217, 89));

    enableFlag = false;
}


LevelHUD::~LevelHUD() {
    delete pauseButtonSprite;
    delete popUpSprite;
    delete noButtonSprite;
    delete yesButtonSprite;
}


void LevelHUD::enablePopUp() {
    enableFlag = !(enableFlag);
}


void LevelHUD::drawHUD(sf::RenderWindow &window) {
    window.draw(*pauseButtonSprite);
    if (enableFlag) {
        window.draw(overlay);
        window.draw(*popUpSprite);
        window.draw(*noButtonSprite);
        window.draw(*yesButtonSprite);
    }
}
