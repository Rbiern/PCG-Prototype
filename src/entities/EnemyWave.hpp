#pragma once
#include <vector>
#include <queue>
#include <SFML/System/Vector2.hpp>
//#include "EnemyEntity.hpp"
#include "Ghoul.hpp"
#include "Bat.hpp"
#include "../core/ResourceManager.hpp"
#include "../json.hpp"


struct WaveConfig {
    int   enemyCount;
    float hpMultiplier;
    float speedMultiplier;
    float spawnInterval;
    int   pathIndex;
};


enum class Events {
    TIME,
    Timed_Delay,
    Enemy_Death,
    ON_DEATH,
    Interval
};


struct spawnConditions {
    float time;
    Events event;
    std::string id;
    EnemyEntity* target;
};


// Comparator for min-heap (earliest 'when' first)
struct ScheduledSpawnCompare {
    bool operator()(const spawnConditions &a, const spawnConditions &b) const {
        return a.time > b.time;
    }
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

private:
    ResourceManager& rm;

    WaveConfig config;
    std::vector<std::vector<sf::Vector2i>> paths;
    float spawnTimer;
    int spawnedSoFar;


    // Active
    std::vector<EnemyEntity*> activeEnemies;
    // Event based spawning
    std::map<std::string, spawnConditions> idMap_;
    // inactive Enemies
    std::priority_queue<spawnConditions,
            std::vector<spawnConditions>,
            ScheduledSpawnCompare> spawnQueue_;





    std::vector<EnemyEntity*> inactiveEnemies;





    std::vector<spawnConditions> allRules_;



    std::unordered_map<std::string, std::vector<spawnConditions>> deathMap_;
    std::unordered_map<std::string, std::vector<spawnConditions>> spawnMap_;
};
