#pragma once
#include <set>
#include "BaseEntity.hpp"
#include "EnemyEntity.hpp"
#include "../levels/tiles/Tile.hpp"


class TowerEntity : public BaseEntity {
protected:
    Direction direction;
    bool isClicked;
    bool piercing;
    int attackDamage;
    float attackCoolDownTimer;
    bool  isFlashing;
    float flashTimer;
    const float flashDuration = 0.1f;
    sf::Texture secondTexture;
    sf::Vector2i gridCoordinates;
    std::vector<sf::Vector2i> tileRange;
    std::vector<sf::RectangleShape> rangeTiles;
    std::vector<sf::RectangleShape> blockedRangeTiles;

public:
    TowerEntity() = default;
    ~TowerEntity() override = default;
    virtual bool OnClick(sf::Vector2i click) = 0;
    virtual void setUserInteraction(bool status) = 0;
    virtual bool isSelected() const = 0;
    virtual void setPosition(const sf::Vector2f& pos) = 0;
    virtual void setScale(sf::Vector2f scale) = 0;
    virtual void setGridCoordinates(sf::Vector2i pos) = 0;
    virtual sf::Vector2i getGridCoordinates() = 0;
    virtual void setDirection(Direction newDir) = 0;
    virtual Direction getDirection() = 0;
    virtual void computeRange(const sf::Vector2i& origin, Direction dir, Tile* grid[16][24]) = 0;
    virtual void update(float deltaTime, const std::vector<EnemyEntity*>& enemies) = 0;
};
