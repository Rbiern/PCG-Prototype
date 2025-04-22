#include "Tower.hpp"


Tower::Tower(const std::string& id) {
    TowerData data = rm.getTowerData(id);
    texture = rm.getTexture(data.imagePath);
    sprite = new sf::Sprite(texture);

    name = data.name;
    attackDamage = data.attackDamage;
    piercing = data.piercing;
    actionSpeed = data.attackCoolDown;
    tileRange = data.tileRange;

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


void Tower::setDirection(orientation newDir) {
    direction = newDir;
}


orientation Tower::getDirection() {
    return direction;
}


void Tower::computeRange(const sf::Vector2i& origin, orientation dir, Tile* grid[12][18]) {
    direction = dir;
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
            case orientation::faceLeft:
            case orientation::faceRight:
                axisKey = offset.x; // block row
                break;
            case orientation::faceUp:
            case orientation::faceDown:
                axisKey = offset.y; // block column
                break;
        }

        // Stop if blocked
        if (!grid[tile.x][tile.y]->canPlace() || blockedTiles.count(axisKey)) {
            blockedTiles.insert(axisKey);

            sf::RectangleShape blockRect(scale);
            blockRect.setPosition(grid[tile.x][tile.y]->getPosition());
            blockRect.setFillColor(sf::Color(255, 0, 0, 120));
            blockRect.setOutlineThickness(1.f);

            blockedRangeTiles.push_back(blockRect);
            continue;
        }

        sf::RectangleShape rect(scale);
        rect.setPosition(grid[tile.x][tile.y]->getPosition());
        rect.setFillColor(sf::Color(255, 255, 0, 100));
        rect.setOutlineColor(sf::Color::Yellow);
        rect.setOutlineThickness(1.f);
        rangeTiles.push_back(rect);
    }
}


bool Tower::update(float deltaTime, sf::Vector2f p) {
    attackCooldown += deltaTime;
    float fireInterval = 1.f / float(actionSpeed);
    if (attackCooldown < fireInterval) {
        return false;
    }

    sf::Vector2f ePos = p;
    bool inRange = false;

    // check each rectangle in rangeTiles
    for (auto& rect : rangeTiles) {
        if (rect.getGlobalBounds().contains(ePos)) {
            inRange = true;
            break;
        }
    }
    if (inRange) {
        return true;
    }
    else return false;
}


void Tower::update(float deltaTime) {}


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


std::vector<sf::Vector2i> Tower::rotateRange(const std::vector<sf::Vector2i>& offsets, orientation dir) {
    std::vector<sf::Vector2i> result;

    for (auto o : offsets) {
        switch (dir) {
            case orientation::faceUp:    result.emplace_back(o.y, -o.x); break;
            case orientation::faceDown:  result.emplace_back(-o.y, o.x); break;
            case orientation::faceRight: result.emplace_back(-o.x, -o.y); break;
            case orientation::faceLeft:  result.emplace_back(o); break;
        }
    }
    return result;
}
