#include "Level.hpp"


Level::Level() : rm(ResourceManager::getInstance()) {
    pauseButton = rm.getTexture("../../images/pause.png");
    towerCountIcon = rm.getTexture("../../images/tower.png");
    remainingEnemiesIcon = rm.getTexture("../../images/e.png");
    font = rm.getFont();

    towerCountText = new sf::Text(font, "Tower Count: " + std::to_string(playerTowerCount), 26);
    towerCountText->setStyle(sf::Text::Bold);
    towerCountText->setFillColor(sf::Color::Black);
    towerCountText->setOutlineColor(sf::Color::White);
    towerCountText->setOutlineThickness(1.f);
    towerCountText->setPosition({310.f + 33.f + 10.f, 22.f});

    remainingText = new sf::Text(font, "Remaining Enemies: " + std::to_string(remainingEnemies), 26);
    remainingText->setStyle(sf::Text::Bold);
    remainingText->setFillColor(sf::Color::Black);
    remainingText->setOutlineColor(sf::Color::White);
    remainingText->setOutlineThickness(1.f);
    remainingText->setPosition({542.f + 30.f + 10.f, 22.f});


    pauseButtonSprite = new sf::Sprite(pauseButton);
    {
        sf::Vector2u size = pauseButton.getSize();
        float scaleX = 50.f / size.x;
        float scaleY = 50.f / size.y;
        pauseButtonSprite->setScale({scaleX, scaleY});
    }
    pauseButtonSprite->setPosition({25.f, 25.f});

    towerCountIconSprite = new sf::Sprite(towerCountIcon);
    {
        sf::Vector2u size = towerCountIcon.getSize();
        float scaleX = 33.f / size.x;
        float scaleY = 33.f / size.y;
        towerCountIconSprite->setScale({scaleX, scaleY});
    }
    towerCountIconSprite->setPosition({310.f, 22.f});

    remainingEnemiesIconSprite = new sf::Sprite(remainingEnemiesIcon);
    {
        sf::Vector2u size = remainingEnemiesIcon.getSize();
        float scaleX = 30.f / size.x;
        float scaleY = 30.f / size.y;
        remainingEnemiesIconSprite->setScale({scaleX, scaleY});
    }
    remainingEnemiesIconSprite->setPosition({542.f, 25.f});


    divider.setSize({6.f, 33.f});
    divider.setFillColor(sf::Color::White);
    divider.setPosition({526.f, 22.f});



    std::array<sf::Vector2f, 8> uiCoords = {
            sf::Vector2f(1102.f, 615.f),
            sf::Vector2f(1194.f, 615.f),
            sf::Vector2f(1102.f, 516.f),
            sf::Vector2f(1194.f, 516.f),
            sf::Vector2f(1102.f, 416.f),
            sf::Vector2f(1194.f, 416.f),
            sf::Vector2f(1102.f, 316.f),
            sf::Vector2f(1194.f, 316.f)
    };

    sf::Color blueColor(0x12, 0x94, 0xD3);  // #1294D3
    sf::Color greenColor(0x20, 0x9F, 0x37); // #209F37
    sf::Color yellowColor(0xF1, 0xCE, 0x00); // #F1CE00
    sf::Color redColor(0xFF, 0x00, 0x00);   // #FF0000

    int i = 0;
    towerSelection[i] = new PlayerCard(uiCoords[0].x, uiCoords[0].y, 12, blueColor);
    towerSelection[i]->towerShape.setTexture(&rm.getTexture("../../images/healer1.png"));
    towerSelection[i++]->towerShape.setSize({135, 135});

    towerSelection[i] = new PlayerCard(uiCoords[1].x, uiCoords[1].y, 12, redColor);
    towerSelection[i]->towerShape.setTexture(&rm.getTexture("../../images/range3.png"));
    towerSelection[i++]->towerShape.setSize({135, 135});

    towerSelection[i] = new PlayerCard(uiCoords[2].x, uiCoords[2].y, 10, redColor);
    towerSelection[i]->towerShape.setTexture(&rm.getTexture("../../images/range1.png"));
    towerSelection[i++]->towerShape.setSize({110, 110});

    towerSelection[i] = new PlayerCard(uiCoords[3].x, uiCoords[3].y, 18, redColor);
    towerSelection[i]->towerShape.setTexture(&rm.getTexture("../../images/range2.png"));
    towerSelection[i++]->towerShape.setSize({135, 135});

    towerSelection[i] = new PlayerCard(uiCoords[4].x, uiCoords[4].y, 12, greenColor);
    towerSelection[i]->towerShape.setTexture(&rm.getTexture("../../images/defense1.png"));
    towerSelection[i++]->towerShape.setSize({135, 135});

    towerSelection[i] = new PlayerCard(uiCoords[5].x, uiCoords[5].y, 12, greenColor);
    towerSelection[i]->towerShape.setTexture(&rm.getTexture("../../images/defense2.png"));
    towerSelection[i++]->towerShape.setSize({135, 135});

    towerSelection[i] = new PlayerCard(uiCoords[6].x, uiCoords[6].y, 17, yellowColor);
    towerSelection[i]->towerShape.setTexture(&rm.getTexture("../../images/fighter1.png"));
    towerSelection[i++]->towerShape.setSize({135, 135});

    towerSelection[i] = new PlayerCard(uiCoords[7].x, uiCoords[7].y, 8, yellowColor);
    towerSelection[i]->towerShape.setTexture(&rm.getTexture("../../images/fighter2.png"));
    towerSelection[i++]->towerShape.setSize({135, 135});


    bool pauseFlag = false;
    playerTowerCount = 0;
    gold = 12;
    remainingEnemies = 0;
    hp = 200;
    waveCount = 1;
}


Level::~Level() {
    delete pauseButtonSprite;
    delete towerCountIconSprite;
    delete remainingEnemiesIconSprite;
    delete towerCountText;
    delete remainingText;
    for (int i = 0; i < 8; ++i) {
        delete towerSelection[i];
    }
}


bool Level::handleUserInput(const sf::Event &event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
            std::cout << "escape\n";
            game->setMenu(std::make_unique<LevelSelection>());
        }
    } else if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (panel.exitSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y))) {
            std::cout << "Button was clicked!" << std::endl;
        }
        if (pauseButtonSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y))) {
            std::cout << "Button was clicked!" << std::endl;
            if (!pauseFlag) {
                pauseFlag = true;
                pauseButtonSprite->setTexture(rm.getTexture("../../images/resume.png"));
            } else {
                pauseFlag = false;
                pauseButtonSprite->setTexture(rm.getTexture("../../images/pause.png"));
            }
        }
    }
    return false;
}


void Level::menuActionUpdate() {

}


void Level::render(sf::RenderWindow &window) {
    // Update the counters
    towerCountText->setString("Tower Count: " + std::to_string(playerTowerCount));
    remainingText->setString("Remaining Enemies: " + std::to_string(remainingEnemies));
    panel.setRound(waveCount);

    // Draw the game gird
    gameField.renderGameFeild(window);

    // Draw UI elements on top of the game grid
    window.draw(*pauseButtonSprite);
    window.draw(*towerCountIconSprite);
    window.draw(*remainingEnemiesIconSprite);
    window.draw(divider);
    window.draw(*towerCountText);
    window.draw(*remainingText);

    // Draw panel left of game grid
    panel.drawSelectionPanel(window);

    // Draw player cards on top of panel
    for (auto & i : towerSelection) {
        i->drawPlayerCard(window);
    }
}
