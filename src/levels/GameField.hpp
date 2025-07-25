#pragma once
#include <SFML/Graphics.hpp>
#include "Generator.hpp"
#include "../entities/Tower.hpp"
//#include "../entities/Ghoul.hpp"
#include "../entities/EnemyWave.hpp"
#include "tiles/TileFactory.hpp"
#include "tiles/TileCreator.cpp"
#include "tiles/Tile.hpp"


class GameField {
public:
    GameField();
    ~GameField();

    bool addPlayerTower(const std::string& id, sf::Vector2f scale, sf::Vector2i pos);
    void deletePlayerTower();
    bool isTowerClick(sf::Vector2i click);
    bool hasSelectedTower();
    void updateTowerDirection();

    void setScale(sf::Vector2f scale);

    int getWaveStat() const;
    int getGoldStat() const;
    int getHPStat() const;
    int getRemainingEnemiesStat() const;

    void update(float dt);
    void renderGameField(sf::RenderWindow& window);

private:
    int rows;
    int cols;
    Tile* grid[16][24];
    Generator gen;

    std::vector<TowerEntity*> playerTowers;
    TowerEntity* selectedTower = nullptr;
    std::unique_ptr<EnemyWave> wave;

    int playerTowerCount;
    // Game level values
    int waveCount;
    int gold;
    int hp;
    int remainingEnemies;

    ResourceManager& rm;
};
