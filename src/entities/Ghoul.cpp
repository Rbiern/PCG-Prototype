#include "Ghoul.hpp"


Ghoul::Ghoul(const std::string& image, const std::vector<sf::Vector2i>& pathTiles, const sf::Vector2f& cellSize) {
    texture = rm.getTexture("../../images/entities/NPC_469.png");
    sprite = new sf::Sprite(texture);
    path = pathTiles;
    scale = cellSize;

    totalFrames = 12;
    frameWidth = 52;
    frameHeight = 60;
    currentFrame = 0;
    animateTimer = 0.f;

    sf::FloatRect bounds = sprite->getLocalBounds();
    float scaleX = scale.x / bounds.size.x;
    float scaleY = scale.y / bounds.size.y;
    float finalScale = std::min(scaleX, scaleY);

    sprite->setTextureRect(sf::IntRect({0, 0}, {frameWidth, frameHeight}));
    sprite->setScale({-sprite->getScale().x, sprite->getScale().y});
//    sprite->setScale({-finalScale, finalScale});
    sprite->setOrigin({frameWidth / 2.f, frameHeight / 2.f}); // Adjust origin so it doesn’t move position
//    sprite->setOrigin({bounds.size.x/2.f, bounds.size.y/2.f});

    health = 30;            // Current health
    maxHealth = 30;         // Max starting health
    killReward = 12;        // Reward for killing enmies
    distanceTraveled = 0.f; // Total distance traveled, used in knowing how is first or last with other enemies
    alive = true;         //
    actionSpeed = 150.f;    // how fast enemy can travel in level
    currentIdx = 0;

    if (!path.empty()) {
        currentPosition = gridToPixelCenter(path[0]);
        sprite->setPosition(currentPosition);
    }
}


Ghoul::~Ghoul() {
    delete sprite;
}


void Ghoul::update(float dt) {
    // --- advance animation timer & frame index as you already do ---
    animateTimer += dt;
    if (animateTimer > 0.09f) {
        currentFrame = (currentFrame + 1) % totalFrames;
//        sprite->setTextureRect(sf::IntRect({0, currentFrame * frameHeight}, {frameWidth, frameHeight}));
        animateTimer = 0.f;
    }

    // --- movement toward next path tile ---
    if (currentIdx >= path.size()) {
        return;
    }

    Direction temp = lastDirection;

    sf::Vector2f target = gridToPixelCenter(path[currentIdx]);
    sf::Vector2f delta = target - currentPosition;
    float dist = std::hypot(delta.x, delta.y);
    float moveDist = actionSpeed * dt;

    if (dist <= moveDist) {
        currentPosition = target;
        ++currentIdx;
    } else {
        // determine direction before normalizing
        if (std::abs(delta.x) > std::abs(delta.y)) {
            lastDirection = (delta.x > 0 ? Direction::faceRight : Direction::faceLeft);
        } else {
            lastDirection = (delta.y > 0 ? Direction::faceDown : Direction::faceUp);
        }
        // normalize & move
        delta /= dist;
        currentPosition += delta * moveDist;
    }
    sprite->setPosition(currentPosition);

    // --- pick the correct row in your sprite-sheet based on lastDirection ---
    switch (lastDirection) {
        case Direction::faceLeft:  if (currentFrame < 7) currentFrame = 7; sprite->setScale({+1.f, 1.f}); break;// Flip horizontally
        case Direction::faceRight: if (currentFrame < 7) currentFrame = 7; sprite->setScale({-1.f, 1.f}); break;// Flip horizontally
        case Direction::faceUp:    if (currentFrame > 5) currentFrame = 2; sprite->setScale({1.f, +1.f}); break;// Flip vertically
        case Direction::faceDown:  if (currentFrame > 5) currentFrame = 2; sprite->setScale({1.f, -1.f}); break;// Flip vertically
    }

    // and each direction is one row of the sheet:
    sprite->setTextureRect(sf::IntRect({0, currentFrame * frameHeight}, {frameWidth, frameHeight}));
}


bool Ghoul::hasFinishedPath() const {
    return currentIdx >= path.size();
}


void Ghoul::takeDamage(int dmg) {
    health -= dmg;
    if (health <= 0) alive = false;
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


void Ghoul::setHealth(int hp) {
    health   = hp;
    alive  = (health > 0);
}


int Ghoul::getMaxHealth() const {
    return maxHealth;
}


void Ghoul::setMaxHealth(int maxHP) {
    maxHealth = maxHP;
}


void Ghoul::setSpeed(float speed) {
    actionSpeed = speed;
}


float Ghoul::getSpeed() const {
    return actionSpeed;
}


void Ghoul::reset() {
    health = maxHealth;
    alive = true;
    distanceTraveled = 0.f;
    currentIdx  = 0;
    if (!path.empty()) {
        currentPosition = gridToPixelCenter(path[0]);
        sprite->setPosition(currentPosition);
    }
}


void Ghoul::setKillReward(int gold) {
    killReward = gold;
}


bool Ghoul::checkIfAlive() {
    return alive;
}
