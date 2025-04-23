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
    std::array<std::string, 8> id = {
            "o1", "o2", "o3", "o4", "o5", "o6", "o7", "o8"
    };
    for (int i = 0; i < 8; ++i) {
        towerSelection[i] = new PlayerCard(uiCoords[i].x, uiCoords[i].y, id[i]);
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
    if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2i pixelClick = {mouseButtonPressed->position.x, mouseButtonPressed->position.y};
        sf::Vector2f worldClick = sf::Vector2f(pixelClick);  // or use mapPixelToCoords if you're using a view

        bool clickedOnUI = false;
        // Panel buttons
        if (panel.isVisible) {
            if (panel.deleteSprite->getGlobalBounds().contains(worldClick)) {
                gameField.deletePlayerTower();
                panel.isVisible = false;
                clickedOnUI = true;
            } else if (panel.directionSprite->getGlobalBounds().contains(worldClick)) {
                gameField.updateTowerDirection();
                clickedOnUI = true;
            }
        }
        // HUD and other UI buttons
        if (panel.exitSprite->getGlobalBounds().contains(worldClick)) {
            exitFlag = true;
            hud.enablePopUp();
            clickedOnUI = true;
        }
        // Pause and resume button
        if (hud.pauseButtonSprite->getGlobalBounds().contains(worldClick)) {
            pauseFlag = !pauseFlag;
            hud.pauseButtonSprite->setTexture(rm.getTexture(pauseFlag ? "../../images/resume.png" : "../../images/pause.png"));
            clickedOnUI = true;
        }
        // Exit game pop up
        if (exitFlag) {
            if (hud.yesButtonSprite->getGlobalBounds().contains(worldClick)) {
                game->setMenu(std::make_unique<LevelSelection>());
                return false;
            }
            if (hud.noButtonSprite->getGlobalBounds().contains(worldClick)) {
                exitFlag = false;
                hud.enablePopUp();
                clickedOnUI = true;
            }
        }
        // Player card selection (towers)
        for (int i = 0; i < 8; i++) {
            if (towerSelection[i]->backgroundRect.getGlobalBounds().contains(worldClick)) {
                currTowerSel = i;
                isDragging = true;
                draggedSprite = new sf::Sprite(rm.getTexture(towerSelection[i]->imagePath));
                setScale();
                clickedOnUI = true;
            }
        }
        // Check if player clicked on tower
        if (!clickedOnUI) {
            bool clickedTower = gameField.isTowerClick(pixelClick);
            if (clickedTower) { //show panel when a tower is selected (even a new one)
                panel.isVisible = true;
            } else {            // clicked on empty space
                panel.isVisible = false;
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

        if (dropPos.x >= 0.f && dropPos.x < data.gridWidth && dropPos.y >= 0.f && dropPos.y < data.gridHeight) {
            if (gameField.addPlayerTower(towerSelection[currTowerSel]->towerId, draggedSprite->getScale(), {row, col})) {
                panel.isVisible = true;
            }
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
    gameField.update(delta);
}


void Level::render(sf::RenderWindow &window) {
    // Update the counters
    panel.setRoundNumber(gameField.getWaveStat());
    panel.setGoldNumber(gameField.getGoldStat());
    panel.setHealthNumber(gameField.getHPStat());
    panel.setRemainingNumber(gameField.getRemainingEnemiesStat());
    // Draw the game grid
    gameField.renderGameField(window);
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
    sf::FloatRect bounds = draggedSprite->getLocalBounds();
    draggedSprite->setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});

    float scaleX = data.squareWidth / bounds.size.x;
    float scaleY = data.squareHeight / bounds.size.y;
    float finalScale = std::min(scaleX, scaleY);
    draggedSprite->setScale({finalScale, finalScale});
}
