#pragma once
#include <utility>

#include "BaseEntity.hpp"


class EnemyEntity : public BaseEntity {
protected:
    int health;
    int maxHealth;
    int killReward;
    bool alive;
    float distanceTraveled;
    size_t currentIdx;
    sf::Vector2f currentPosition;
    std::vector<sf::Vector2i> path;
    sf::RectangleShape healthBar;
    sf::RectangleShape healthBarBackground;
    sf::Vector2f gridToPixelCenter(const sf::Vector2i& tile) const {
        int row = tile.x;
        int col = tile.y;
        return {
                row * scale.y + scale.y * 0.5f,
                col * scale.x + scale.x * 0.5f
        };
    }

public:
    EnemyEntity() = default;
    virtual ~EnemyEntity() = default;
    virtual void takeDamage(int dmg) = 0;
    virtual float getDistanceTraveled() const = 0;
    virtual void setPosition(const sf::Vector2f& pos) = 0;
    virtual sf::Vector2f getPosition() = 0;
    virtual void setKillReward(int gold) = 0;
    virtual void setHealth(int hp) = 0;
    virtual int getMaxHealth() const = 0;
    virtual void setMaxHealth(int maxHP) = 0;
    virtual void setSpeed(float speed) = 0;
    virtual float getSpeed() const = 0;
    virtual bool checkIfAlive() = 0;
    virtual void reset() = 0;
    virtual void update(float dt) = 0;

    void setID(std::string id) {
        name = std::move(id);
    }
    std::string getID() {
        return name;
    }
};
