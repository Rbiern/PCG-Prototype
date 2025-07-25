#include "Tower.hpp"


Tower::Tower(const std::string& id) {
    TowerData data = rm.getTowerData(id);
    texture = rm.getTexture(data.towerImagePath_a);
    secondTexture = rm.getTexture(data.towerImagePath_b);
    sprite = new sf::Sprite(texture);

    name = data.name;
    attackDamage = data.ATK;
    piercing = false;
    actionSpeed = data.attackInterval;
    tileRange = data.tileRange;
    attackCoolDownTimer = .0f;
    isFlashing = false;
    flashTimer = 0.f;

    scale = {rm.getGridData().squareWidth, rm.getGridData().squareHeight};
    isClicked = false;
}


Tower::~Tower() {
    delete sprite;
}


bool Tower::OnClick(sf::Vector2i click) {
    if (sprite->getGlobalBounds().contains(sf::Vector2f(click.x, click.y)) ) {
        return true;
    }
    return false;
}


void Tower::setUserInteraction(bool status) {
    isClicked = status;
}


bool Tower::isSelected() const {
    return isClicked;
}


void Tower::setPosition(const sf::Vector2f& pos) {
    sprite->setPosition(pos);
}


void Tower::setScale(sf::Vector2f scale) {
    sprite->setScale(scale);
}


void Tower::setGridCoordinates(sf::Vector2i pos) {
    gridCoordinates = pos;
}


sf::Vector2i Tower::getGridCoordinates() {
    return gridCoordinates;
}


void Tower::setDirection(Direction newDir) {
    direction = newDir;
}


Direction Tower::getDirection() {
    return direction;
}


void Tower::computeRange(const sf::Vector2i& origin, Direction dir, Tile* grid[16][24]) {
    direction = dir;
    auto lb = sprite->getLocalBounds();
    sprite->setOrigin({lb.position.x   + lb.size.x  * 0.5f, lb.position.y    + lb.size.y * 0.5f});
    switch (dir) {
        case Direction::faceLeft:
            sprite->setScale({abs(sprite->getScale().x), sprite->getScale().y});
            break;
        case Direction::faceRight:
            sprite->setScale({-sprite->getScale().x, sprite->getScale().y});
            break;
        default:
            break;
    }
    sf::Vector2f cellPos = grid[origin.x][origin.y]->getPosition();
    sprite->setPosition({cellPos.x + rm.getGridData().squareWidth * 0.5f,cellPos.y + rm.getGridData().squareHeight * 0.5f});

    // Rest range tiles if previously computed
    if (!rangeTiles.empty()) {
        rangeTiles.clear();
    }
    // Rest blocked range tiles if previously computed
    if (!blockedRangeTiles.empty()) {
        blockedRangeTiles.clear();
    }
    gridCoordinates = origin;

    auto rotated = rotateRange(tileRange, dir);
    std::set<int> blockedTiles;

    for (const auto& offset : rotated) {
        sf::Vector2i tile = origin + offset;
        // Check Bounds
        if (tile.x < 0 || tile.x >= rm.getGridData().rows || tile.y < 0 || tile.y >= rm.getGridData().cols) {
            continue;
        }
        // Determine what axis to block based on orientation
        int axisKey = 0;
        switch (dir) {
            case Direction::faceLeft:
            case Direction::faceRight:
                axisKey = offset.x; // block row
                break;
            case Direction::faceUp:
            case Direction::faceDown:
                axisKey = offset.y; // block column
                break;
        }
        // Stop if blocked
        if (grid[tile.x][tile.y]->isBlockRange() || blockedTiles.count(axisKey)) {
            blockedTiles.insert(axisKey);

            sf::RectangleShape blockRect(scale);
            blockRect.setPosition(grid[tile.x][tile.y]->getPosition());
            blockRect.setFillColor(sf::Color(255, 0, 0, 120)); // red
            blockRect.setOutlineThickness(1.f);

            blockedRangeTiles.push_back(blockRect);
            continue;
        }

        sf::RectangleShape rect(scale);
        rect.setPosition(grid[tile.x][tile.y]->getPosition());
        rect.setFillColor(sf::Color(255, 255, 0, 100)); // yellow
        rect.setOutlineColor(sf::Color::Yellow);
        rect.setOutlineThickness(1.f);
        rangeTiles.push_back(rect);
    }
}


void Tower::update(float dt, const std::vector<EnemyEntity*>& enemies) {
    if (isFlashing) {
        flashTimer += dt;
        if (flashTimer >= flashDuration) {
            sprite->setTexture(texture, true);
            isFlashing = false;
        }
    }
    attackCoolDownTimer += dt;
    float attackInterval = 1.f / actionSpeed;
    if (attackCoolDownTimer < attackInterval) {
        return;
    }

    std::vector<EnemyEntity*> inRange;
    for (auto* e : enemies) {
        sf::Vector2f ep = e->getPosition();
        for (auto& rect : rangeTiles) {
            if (rect.getGlobalBounds().contains(ep)) {
                inRange.push_back(e);
                break;
            }
        }
    }
    if (inRange.empty()) {
        return;
    }

    if (piercing) {
        for (auto* e : inRange) {
            e->takeDamage(attackDamage);
        }
    } else {
        auto* target = *std::max_element(inRange.begin(), inRange.end(), [](EnemyEntity* a, EnemyEntity* b) {
            return a->getDistanceTraveled() < b->getDistanceTraveled();
        });
        target->takeDamage(attackDamage);
    }
    attackCoolDownTimer = 0.f;
    isFlashing = true;
    flashTimer = 0.f;
    sprite->setTexture(secondTexture, true);
}


void Tower::render(sf::RenderWindow& window) {
    // Draw the tower sprite
    window.draw(*sprite);
    // Draw the in range and blocked tiles
    if (isClicked) {
        for (const auto &tilePos: rangeTiles) {
            window.draw(tilePos);
        }
        for (const auto &tilePos: blockedRangeTiles) {
            window.draw(tilePos);
        }
    }
}


std::vector<sf::Vector2i> Tower::rotateRange(const std::vector<sf::Vector2i>& offsets, Direction dir) {
    std::vector<sf::Vector2i> result;
    for (auto o : offsets) {
        switch (dir) {
            case Direction::faceUp:    result.emplace_back(o.y, -o.x); break;
            case Direction::faceDown:  result.emplace_back(-o.y, o.x); break;
            case Direction::faceRight: result.emplace_back(-o.x, -o.y); break;
            case Direction::faceLeft:  result.emplace_back(o); break;
        }
    }
    return result;
}
