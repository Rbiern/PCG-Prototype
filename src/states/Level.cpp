#include "Level.hpp"


Level::Level() : rm(ResourceManager::getInstance()) {
    if (!rm.loadTowerData("../../towers.json")) {
        std::cerr << "Creating UI Error: Cannot open JSON file.\n" << std::endl;
        exit(-1);
    }
    // List of coords to place tower selection cards on panel
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
    // List of all tower names from the JSON
    std::array<std::string, 8> towerNames = {
            "o1", "o2", "o3", "o4", "o5", "o6", "o7", "o8"
    };
    for (int i = 0; i < 8; ++i) {
        towerSelection[i] = new PlayerCard(uiCoords[i].x, uiCoords[i].y, towerNames[i]);
    }
    // Flags
    pauseFlag = false;
    exitFlag = false;
    isDragging = false;
}


Level::~Level() {
    for (auto & i : towerSelection) {
        delete i;
    }
}


bool Level::handleUserInput(const sf::Event &event) {
    /** Check for keyboard input */
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
            exitFlag = !exitFlag;
            hud.enablePopUp();
        }
    }
    /** Check for mouse input */
    else if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        mouseButtonPress = {mouseButtonPressed->position.x, mouseButtonPressed->position.y};
        if (gameField.isTowerClick(mouseButtonPress)) {
            panel.isVisible = !(panel.isVisible);
        }
        if (panel.isVisible) {
            if (panel.deleteSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y))) {
                gameField.deletePlayerTower();
                panel.isVisible = false;
            }
            if (panel.directionSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y))) {
                gameField.updateTowerDir();
                std::cout << "Button was clicked!" << std::endl;
            }
        }
        // Exit button
        if (panel.exitSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y))) {
            std::cout << "Button was clicked!" << std::endl;
            exitFlag = true;
            hud.enablePopUp();
        }
        // Pause and resume button
        if (hud.pauseButtonSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y))) {
            std::cout << "Button was clicked!" << std::endl;
            if (!pauseFlag) {
                pauseFlag = true;
                hud.pauseButtonSprite->setTexture(rm.getTexture("../../images/resume.png"));
            } else {
                pauseFlag = false;
                hud.pauseButtonSprite->setTexture(rm.getTexture("../../images/pause.png"));
            }
        }
        // Exit game pop up
        if (exitFlag) {
            if (hud.yesButtonSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y))) {
                game->setMenu(std::make_unique<LevelSelection>());
                return false;
            }
            if (hud.noButtonSprite->getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y))) {
                exitFlag = false;
                hud.enablePopUp();
            }
        }
        // Player cards
        for (int i = 0; i < 8; i++) {
            if (towerSelection[i]->backgroundRect.getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y))) {
                currTowerSel = i;
                isDragging = true;
                draggedSprite = new sf::Sprite(rm.getTexture(towerSelection[i]->imagePath));
                setScale();
            }
        }
    }
    /** Check if mouse is moving and dragging */
    if (isDragging && draggedSprite != nullptr) {
        if (const auto* mouseButtonMove = event.getIf<sf::Event::MouseMoved>()) {
            sf::Vector2f newMousePos(static_cast<float>(mouseButtonMove->position.x), static_cast<float>(mouseButtonMove->position.y));
            draggedSprite->setPosition(newMousePos);
        }
    }
    /** Check if mouse stopped dragging */
    if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (!isDragging) {
            goto end_if;
        } else {
            isDragging = false;
        }

        gridConfig data = rm.getGridData();
        sf::Vector2f dropPos(static_cast<float>(mouseButtonReleased->position.x), static_cast<float>(mouseButtonReleased->position.y));
        int col = static_cast<int>(dropPos.x / data.squareWidth);
        int row = static_cast<int>(dropPos.y / data.squareHeight);

        std::cout << "Dropped at grid cell: <" << col << ", " << row << ">" << std::endl;
        if (dropPos.x >= 0.f && dropPos.x < data.gridWidth && dropPos.y >= 0.f && dropPos.y < data.gridHeight) {
            gameField.addPlayerTower(towerSelection[currTowerSel]->imagePath, draggedSprite->getScale(), {row, col});
        }
        delete draggedSprite;
    }
    end_if:

    return false;
}


void Level::menuActionUpdate(float delta) {
    if (pauseFlag) {
        return;
    }
    gameField.run(delta);
}


void Level::render(sf::RenderWindow &window) {
    // Update the counters
    panel.setRoundNumber(gameField.getWaveStat());
    panel.setGoldNumber(gameField.getGoldStat());
    panel.setHealthNumber(gameField.getHPStat());
    panel.setRemainingNumber(gameField.getRemainingEnemiesStat());
    // Draw the game grid
    gameField.renderGameFeild(window);
    panel.drawPanel(window);
    // Draw player cards on top of panel
    for (auto & i : towerSelection) {
        i->drawPlayerCard(window);
    }
    if (isDragging) {
        window.draw(*draggedSprite);
    }
    hud.drawHUD(window);
}


void Level::setScale() {
    gridConfig data = rm.getGridData();
    // Set origin to center so scaling/positioning is smooth
    sf::FloatRect bounds = draggedSprite->getLocalBounds();
    draggedSprite->setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    // Calculate scaling to preserve aspect ratio and fit within the cell
    float scaleX = data.squareWidth / bounds.size.x;
    float scaleY = data.squareHeight / bounds.size.y;
    float finalScale = std::min(scaleX, scaleY); // Use the smaller scale factor
    // Set scale of sprite to be dragged around
    draggedSprite->setScale({finalScale, finalScale});
}
