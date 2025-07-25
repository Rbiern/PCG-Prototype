#pragma once
#include <SFML/Graphics.hpp>
#include "../core/ResourceManager.hpp"

enum class Direction {
    faceLeft,
    faceDown,
    faceRight,
    faceUp
};

class BaseEntity {
protected:
    sf::Texture texture;
    sf::Sprite* sprite;
    std::string name;
    sf::Vector2f scale;
    Direction lastDirection;
    float actionSpeed;
    ResourceManager& rm;

public:
    BaseEntity() : rm(ResourceManager::getInstance()) {}
    virtual ~BaseEntity() = default;
    virtual void render(sf::RenderWindow& window) = 0;
};
