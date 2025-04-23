#include "GameField.hpp"


GameField::GameField() : rm(ResourceManager::getInstance()) {
    rows = rm.getGridData().rows;
    cols = rm.getGridData().cols;

    int map[12][18];
    const int (&src)[Generator::Rows][Generator::Cols] = gen.getMap();
    for (int r = 0; r < Generator::Rows; ++r) {
        for (int c = 0; c < Generator::Cols; ++c) {
            map[r][c] = src[r][c];
        }
    }
    // Terrain Grid spans the entire length and 85% width of the screen
    float gridWidth = 1280 * .85f;
    float gridHeight = 720;
    // The size of each square in the grid
    float squareWidth = gridWidth / cols;
    float squareHeight = gridHeight / rows;
    // Fill the grid matrix with tiles
    TileFactory* node = new TileCreator();
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            grid[row][col] = node->createTower(map[row][col]);

            float x = col * squareWidth;
            float y = row * squareHeight;

            grid[row][col]->setPosition({x, y});
            grid[row][col]->tile.setSize(sf::Vector2f(squareWidth, squareHeight));
        }
    }
    delete node;

    std::vector<std::vector<sf::Vector2i>> allPaths;
    allPaths.push_back(gen.getPath());
    waveCount = 1;
    WaveConfig cfg{
            /* enemyCount: */ 10,
            /* hpMult:     */ 1.0f,
            /* speedMult:  */ 1.0f,
            /* spawnIntv:  */ 1.0f,
            /* pathIndex:  */ 0 };
    wave = std::make_unique<EnemyWave>(cfg, allPaths, rm);
    gold = 16;
    hp = 100;
    remainingEnemies = 0;
}


GameField::~GameField() {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            delete grid[row][col];
            grid[row][col] = nullptr;
        }
    }
}


bool GameField::addPlayerTower(const std::string& id, sf::Vector2f scale, sf::Vector2i pos) {
    // Check if tower can be placed on tile
    if (!grid[pos.x][pos.y]->canPlaceTower) {
        return false;
    }
    TowerEntity* towerNode = new Tower(id);
    towerNode->setPosition(grid[pos.x][pos.y]->tile.getPosition());
    towerNode->setGridCoordinates(pos);
    towerNode->setScale(scale);
    towerNode->computeRange(pos, orientation::faceLeft, grid);

    if (selectedTower != nullptr) {             // De-select any selected towers
        selectedTower->setUserInteraction(false);
    }
    selectedTower = towerNode;                  // Cache selected tower
    towerNode->setUserInteraction(true);        // User my interact with the tower and make in game changes under this status
    playerTowers.emplace_back(towerNode);       // Add newly placed tower to list of all towers on field
    grid[pos.x][pos.y]->canPlaceTower = false;  // No other tower can be placed on the same tile

    return true;
}


void GameField::deletePlayerTower() {
    if (selectedTower == nullptr) {
        return;
    }
    sf::Vector2i gridPos = selectedTower->getGridCoordinates();
    grid[gridPos.x][gridPos.y]->canPlaceTower = true;

    for (auto it = playerTowers.begin(); it != playerTowers.end(); ) {
        if ((*it)->isSelected()) {
            it = playerTowers.erase(it);
        } else {
            ++it;
        }
    }
    selectedTower = nullptr;
}


bool GameField::isTowerClick(sf::Vector2i click) {
    for (const auto& towers : playerTowers) {
        if (towers->OnClick(click)) {
            if (selectedTower == towers) {
                towers->setUserInteraction(false);
                selectedTower = nullptr;
            } else {
                if (selectedTower != nullptr) {
                    selectedTower->setUserInteraction(false);
                }
                towers->setUserInteraction(true);
                selectedTower = towers;
            }
            return true;
        }
    }
    // No tower clicked then clear selection if any
    if (selectedTower != nullptr) {
        selectedTower->setUserInteraction(false);
        selectedTower = nullptr;
    }
    return false;
}


void GameField::updateTowerDirection() {
    for (const auto &towers : playerTowers) {
        if (towers->isSelected()) {
            int temp = static_cast<int>(towers->getDirection());
            temp = (temp + 1) % 4;
            orientation dir = static_cast<orientation>(temp);

            towers->setDirection(dir);
            towers->computeRange(towers->getGridCoordinates(), dir, grid);
        }
    }
}


int GameField::getWaveStat() const {
    return waveCount;
}


int GameField::getGoldStat() const {
    return gold;
}


int GameField::getHPStat() const {
    return hp;
}


int GameField::getRemainingEnemiesStat() const {
    return remainingEnemies;
}


void GameField::update(float dt) {
    wave->update(dt);
    remainingEnemies = wave->getRemainingCount();

    auto& enemies = wave->getActiveEnemies();
    for (auto* tower : playerTowers) {
        tower->update(dt, enemies);
    }
}


void GameField::renderGameField(sf::RenderWindow &window) {
    // Draw grid tiles
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (grid[row][col] != nullptr) {
                window.draw(grid[row][col]->tile);
            }
        }
    }
    // Draw towers placed on the grid
    if (!playerTowers.empty()) {
        for (const auto &towers: playerTowers) {
            towers->render(window);
        }
    }
    // Draw all active enemies:
    for (auto* e : wave->getActiveEnemies()) {
        e->render(window);
    }
}
