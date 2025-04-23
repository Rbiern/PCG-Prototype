#pragma once
#include "Tile.hpp"

class TileFactory {
public:
    virtual ~TileFactory() = default;
    virtual Tile* createTower(int id) = 0;
};
