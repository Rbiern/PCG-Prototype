#include "GameField.hpp"


GameField::GameField() : rm(ResourceManager::getInstance()) {
    int map[8][12] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0},
            {0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}
    };

    // Fill the grid matrix with tiles
    float gridPercentWidth = 0.85f;

    float gridWidth = 1280 * gridPercentWidth;
    float gridHeight = 720;

    float squareWidth = gridWidth / cols;
    float squareHeight = gridHeight / rows;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (map[row][col] == 0)
                grid[row][col] = new GrassTile();
            else
                grid[row][col] = new PathTile();

            float x = col * squareWidth;
            float y = row * squareHeight;

            grid[row][col]->setPosition({x, y});
            grid[row][col]->tile.setSize(sf::Vector2f(squareWidth, squareHeight));
        }
    }
}


GameField::~GameField() {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            delete grid[row][col];
            grid[row][col] = nullptr;
        }
    }
}


void GameField::addPlayerTower(const std::string& id, sf::Vector2f scale, sf::Vector2i pos) {

}


void GameField::renderGameFeild(sf::RenderWindow &window) {
    // Draw grid tiles
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (grid[row][col] != nullptr) {
                window.draw(grid[row][col]->tile);
            }
        }
    }
    // In your render function or draw loop:
    if (!playerTowers.empty()) {
        for (const auto &sprite: playerTowers) {
            window.draw(sprite);
        }
    }
}
