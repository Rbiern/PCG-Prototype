#pragma once
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "EnemyEntity.hpp"
#include "../core/ResourceManager.hpp"


struct WaveConfig {
    int   enemyCount;
    float hpMultiplier;
    float speedMultiplier;
    float spawnInterval;
    int   pathIndex;
};


class EnemyWave {
public:
    EnemyWave(const WaveConfig& cfg, const std::vector<std::vector<sf::Vector2i>>& paths,ResourceManager& rm);
    ~EnemyWave();
    void update(float dt);
    void reset(const WaveConfig& cfg);
    bool isFinished() const;
    int getRemainingCount() const;
    const std::vector<EnemyEntity*>& getActiveEnemies() const;

private:
    void spawnNext();

    WaveConfig  config;
    ResourceManager& rm;
    std::vector<std::vector<sf::Vector2i>> paths;
    std::vector<EnemyEntity*> inactiveEnemies;
    std::vector<EnemyEntity*> activeEnemies;
    float spawnTimer;
    int spawnedSoFar;
};
