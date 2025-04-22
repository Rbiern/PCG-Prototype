#include "Ghoul.hpp"


Ghoul::Ghoul(const std::string& image, const std::vector<sf::Vector2i>& pathTiles, const sf::Vector2f& cellSize) : path(pathTiles), tileSize(cellSize) {
    texture = rm.getTexture(image);
    sprite = new sf::Sprite(texture);

    sf::FloatRect bounds = sprite->getLocalBounds();
    float scaleX = tileSize.x / bounds.size.x;
    float scaleY = tileSize.y / bounds.size.y;
    float finalScale = std::min(scaleX, scaleY);

    sprite->setScale({finalScale, finalScale});
    sprite->setOrigin({bounds.size.x/2.f, bounds.size.y/2.f});

    actionSpeed = 2.1;
    health = 45;
    maxHealth = 45;
    distanceTraveled = 0.f;
    killReward = 0;
    actionSpeed = 20.f;
    isAlive = true;

    if (!path.empty()) {
        currentPosition = tileToWorld(path[0]);
        sprite->setPosition(currentPosition);
    }
}


Ghoul::~Ghoul() {
    delete sprite;
}


void Ghoul::update(float dt) {
    if (currentIdx >= path.size()) {
        return;
    }

    sf::Vector2f target = tileToWorld(path[currentIdx]);
    sf::Vector2f delta = target - currentPosition;

    float dist = std::hypot(delta.x, delta.y);
    float moveDist = actionSpeed * dt;

    if (dist <= moveDist) {
        currentPosition = target;
        ++currentIdx;
    } else {
        delta /= dist;
        currentPosition += delta * moveDist;
    }

    sprite->setPosition(currentPosition);
}


bool Ghoul::hasFinishedPath() const {
    return currentIdx >= path.size();
}


void Ghoul::takeDamage(int dmg) {
    health -= dmg;
    if (health <= 0) isAlive = false;
}


float Ghoul::getDistanceTraveled() const {
    return distanceTraveled;
}


void Ghoul::setPosition(const sf::Vector2f& pos) {
    sprite->setPosition(pos);
}


sf::Vector2f Ghoul::getPosition() {
    return sprite->getPosition();
}


void Ghoul::render(sf::RenderWindow& window) {
    // Draw enemy
    window.draw(*sprite);
    // Draw health bar
    const float barW = sprite->getGlobalBounds().size.x;
    const float barH = 5.f;
    healthBarBackground.setSize({barW, barH});
    healthBarBackground.setPosition({currentPosition.x - barW/2, currentPosition.y - sprite->getGlobalBounds().size.y/2 - 10});
    healthBarBackground.setFillColor({50,50,50,200});
    window.draw(healthBarBackground);

    float pct = std::max(0.f, static_cast<float>(health)) / static_cast<float>(maxHealth);
    healthBar.setSize({barW * pct, barH});
    healthBar.setPosition(healthBarBackground.getPosition());
    healthBar.setFillColor({static_cast<uint8_t>(((1-pct)*255)), static_cast<uint8_t>((pct*255)), 0, 255});
    window.draw(healthBar);
}


sf::Vector2f Ghoul::tileToWorld(const sf::Vector2i& t) const {
    int row = t.x, col = t.y;
    return {
            row * tileSize.y + tileSize.y * 0.5f,
            col * tileSize.x + tileSize.x * 0.5f

    };
}
