#pragma once
#include <SFML/Graphics.hpp>
#include "../../core/ResourceManager.hpp"

class GameField;

class Tile {
    friend class GameField;
protected:
    sf::Texture* texture;
    sf::RectangleShape tile;
    bool canPlaceTower;
    bool blockRange;
    bool spawn;
    bool despawn;

public:
    virtual ~Tile() = default;

    bool canPlace() const {
        return canPlaceTower;
    }

    bool isBlockRange() const {
        return blockRange;
    }

    bool isSpawn() const {
        return spawn;
    }

    virtual bool isDespawn() const {
        return despawn;
    }

    void setPosition(const sf::Vector2f& pos) {
        tile.setPosition(pos);
    }

    sf::Vector2f getPosition() {
        return tile.getPosition();
    }
};


class GrassTile : public Tile {
private:
    ResourceManager& rm;

public:
    GrassTile() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/grass 0.png");
        tile.setTexture(texture);
        canPlaceTower = true;
        blockRange = false;
        spawn = despawn = false;
    }
};


class PathTile : public Tile {
private:
    ResourceManager& rm;

public:
    PathTile() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/grasscorner 0.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = true;
    }
};


class PathTurnTileA : public Tile {
private:
    ResourceManager& rm;

public:
    PathTurnTileA() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/roadturn 0.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = true;
    }
};


class PathTurnTileB : public Tile {
private:
    ResourceManager& rm;

public:
    PathTurnTileB() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/roadturn 1.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = true;
    }
};


class PathTurnTileC : public Tile {
private:
    ResourceManager& rm;

public:
    PathTurnTileC() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/roadturn 2.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = true;
    }
};


class PathTurnTileD : public Tile {
private:
    ResourceManager& rm;

public:
    PathTurnTileD() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/roadturn 3.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = true;
    }
};


class RockTile : public Tile {
private:
    ResourceManager& rm;

public:
    RockTile() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/rock.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = true;
        spawn = despawn = false;
    }
};


class WaterTileA : public Tile {
private:
    ResourceManager& rm;

public:
    WaterTileA() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/water_a 0.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = false;
    }
};


class WaterTileB : public Tile {
private:
    ResourceManager& rm;

public:
    WaterTileB() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/water_b 0.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = false;
    }
};


class WaterTileC : public Tile {
private:
    ResourceManager& rm;

public:
    WaterTileC() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/water_c 0.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = false;
    }
};


class WaterTurnTileA : public Tile {
private:
    ResourceManager& rm;

public:
    WaterTurnTileA() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/waterturn 0.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = false;
    }
};


class WaterTurnTileB : public Tile {
private:
    ResourceManager& rm;

public:
    WaterTurnTileB() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/waterturn 1.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = false;
    }
};


class WaterTurnTileC : public Tile {
private:
    ResourceManager& rm;

public:
    WaterTurnTileC() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/waterturn 2.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = false;
    }
};


class WaterTurnTileD : public Tile {
private:
    ResourceManager& rm;

public:
    WaterTurnTileD() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/waterturn 3.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = false;
    }
};


class WaterSideTileA : public Tile {
private:
    ResourceManager& rm;

public:
    WaterSideTileA() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/waterside 0.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = false;
    }
};


class WaterSideTileB : public Tile {
private:
    ResourceManager& rm;

public:
    WaterSideTileB() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/waterside 1.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = false;
    }
};


class WaterSideTileC : public Tile {
private:
    ResourceManager& rm;

public:
    WaterSideTileC() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/waterside 2.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = false;
    }
};


class WaterSideTileD : public Tile {
private:
    ResourceManager& rm;

public:
    WaterSideTileD() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/waterside 3.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = false;
    }
};


class WaterCornerTileA : public Tile {
private:
    ResourceManager& rm;

public:
    WaterCornerTileA() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/watercorner 0.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = false;
    }
};


class WaterCornerTileB : public Tile {
private:
    ResourceManager& rm;

public:
    WaterCornerTileB() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/watercorner 1.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = false;
    }
};


class WaterCornerTileC : public Tile {
private:
    ResourceManager& rm;

public:
    WaterCornerTileC() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/watercorner 2.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = false;
    }
};


class WaterCornerTileD : public Tile {
private:
    ResourceManager& rm;

public:
    WaterCornerTileD() : rm(ResourceManager::getInstance()) {
        texture = &rm.getTexture("../../New folder/watercorner 3.png");
        tile.setTexture(texture);
        canPlaceTower = false;
        blockRange = false;
        spawn = despawn = false;
    }
};
