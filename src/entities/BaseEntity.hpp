#pragma once
#include <SFML/Graphics.hpp>
#include "../core/ResourceManager.hpp"

enum class orientation {
    faceLeft,
    faceDown,
    faceRight,
    faceUp
};

class BaseEntity {
protected:
    sf::Texture texture;
    sf::Sprite* sprite;
    sf::Vector2f scale;
    sf::Vector2i gridCoordinates;
    std::string name;
    float actionSpeed;
    ResourceManager& rm;

public:
    BaseEntity() : rm(ResourceManager::getInstance()) {}
    virtual ~BaseEntity() = default;
    virtual void render(sf::RenderWindow& window) = 0;
};
