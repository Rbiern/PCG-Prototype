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

public:
    EnemyEntity() = default;
    virtual ~EnemyEntity() = default;
    virtual void takeDamage(int dmg) = 0;
    virtual float getDistanceTraveled() const = 0;
    virtual void setPosition(const sf::Vector2f& pos) = 0;
    virtual sf::Vector2f getPosition() = 0;
};
