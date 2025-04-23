#include "EnemyWave.hpp"
#include "Ghoul.hpp"


EnemyWave::EnemyWave(const WaveConfig& cfg, const std::vector<std::vector<sf::Vector2i>>& paths, ResourceManager& rm) : config(cfg), rm(rm), paths(paths) {
    for (int i = 0; i < config.enemyCount; ++i) {
        Ghoul* e = new Ghoul("../../images/zombie.png", paths[config.pathIndex], {rm.getGridData().squareWidth, rm.getGridData().squareHeight});
        e->setHealth(e->getMaxHealth() * config.hpMultiplier);
        e->setMaxHealth(e->getMaxHealth() * config.hpMultiplier);
        e->setSpeed(e->getSpeed() * config.speedMultiplier);
        inactiveEnemies.push_back(e);
    }
    spawnTimer = 0.f;
    spawnedSoFar = 0;
}


EnemyWave::~EnemyWave() {
    for (auto* e : activeEnemies) delete e;
    for (auto* e : inactiveEnemies) delete e;
}


void EnemyWave::reset(const WaveConfig& cfg) {
    for (auto * e : activeEnemies) delete e;
    for (auto * e : inactiveEnemies) delete e;
    activeEnemies.clear();
    inactiveEnemies.clear();
    config = cfg;
    spawnTimer = 0.f;
    spawnedSoFar = 0;

    for (int i = 0; i < config.enemyCount; ++i) {
        Ghoul* e = new Ghoul("../../images/zombie.png", paths[config.pathIndex], {rm.getGridData().squareWidth, rm.getGridData().squareHeight});
        e->setHealth(e->getMaxHealth() * config.hpMultiplier);
        e->setMaxHealth(e->getMaxHealth() * config.hpMultiplier);
        e->setSpeed(e->getSpeed() * config.speedMultiplier);
        inactiveEnemies.push_back(e);
    }
}


void EnemyWave::spawnNext() {
    if (spawnedSoFar >= config.enemyCount || inactiveEnemies.empty()) {
        return;
    }
    EnemyEntity* e = inactiveEnemies.back();
    inactiveEnemies.pop_back();

    e->reset();
    activeEnemies.push_back(e);
    ++spawnedSoFar;
}


void EnemyWave::update(float dt) {
    spawnTimer += dt;
    if (spawnTimer >= config.spawnInterval) {
        spawnNext();
        spawnTimer = 0.f;
    }

    for (auto it = activeEnemies.begin(); it != activeEnemies.end(); ) {
        EnemyEntity* e = *it;
        e->update(dt);
        if (!e->checkIfAlive()) {
            delete e;
            it = activeEnemies.erase(it);
        } else {
            ++it;
        }
    }
}


bool EnemyWave::isFinished() const {
    return spawnedSoFar >= config.enemyCount && activeEnemies.empty();
}


int EnemyWave::getRemainingCount() const {
    return (config.enemyCount - spawnedSoFar) + activeEnemies.size();
}


const std::vector<EnemyEntity*>& EnemyWave::getActiveEnemies() const {
    return activeEnemies;
}
