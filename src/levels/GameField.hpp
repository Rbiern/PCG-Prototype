#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.hpp"
#include "GrassTile.cpp"
#include "PathTile.cpp"
#include "../entities/Simon.hpp"
#include "../entities/TowerCreator.cpp"


class GameField {
public:
    GameField();
    ~GameField();
    void addPlayerTower(const std::string& id, sf::Vector2f scale, sf::Vector2i pos);
    void renderGameFeild(sf::RenderWindow& window);

private:
    unsigned rows = 8;// 10 12   18
    unsigned cols = 12;//7 8   12
    Tile* grid[8][12];
    std::vector<sf::Sprite> playerTowers;
    ResourceManager& rm;
};
