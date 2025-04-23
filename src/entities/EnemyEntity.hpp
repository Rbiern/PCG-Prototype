#pragma once
#include "BaseEntity.hpp"


class EnemyEntity : public BaseEntity {
protected:
    int killReward;
    int health;
    int maxHealth;
    bool isAlive;
    float distanceTraveled;
    sf::RectangleShape healthBar;
    sf::RectangleShape healthBarBackground;
    sf::Vector2f currentPosition;

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
};
