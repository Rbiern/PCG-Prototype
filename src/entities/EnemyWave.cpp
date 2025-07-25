#include "EnemyWave.hpp"

using json = nlohmann::json;
// spawn at this time
// spawn after this event
//      after enemy died
//      after time interval of certain enemy


// queue has all in active enemies yet to be spawn on field
// vector has all active enemies on feild

// spawn all timing based enemies by pulling out of queue and put into active array
// if one dies then check queue for dead activity



EnemyWave::EnemyWave(const WaveConfig& cfg, const std::vector<std::vector<sf::Vector2i>>& paths, ResourceManager& rm) : config(cfg), rm(rm), paths(paths) {
//    for (int i = 0; i < config.enemyCount; ++i) {
//        Ghoul* e = new Ghoul("../../images/zombie.png", paths[config.pathIndex], {rm.getGridData().squareWidth, rm.getGridData().squareHeight});
//        e->setHealth(e->getMaxHealth() * config.hpMultiplier);
//        e->setMaxHealth(e->getMaxHealth() * config.hpMultiplier);
//        e->setSpeed(e->getSpeed() * config.speedMultiplier);
//        inactiveEnemies.push_back(e);
//    }
//    for (int i = 0; i < config.enemyCount; ++i) {
//        Bat* b = new Bat("../../images/zombie.png", paths[config.pathIndex], {rm.getGridData().squareWidth, rm.getGridData().squareHeight});
//        b->setHealth(b->getMaxHealth() * config.hpMultiplier);
//        b->setMaxHealth(b->getMaxHealth() * config.hpMultiplier);
//        b->setSpeed(b->getSpeed() * config.speedMultiplier);
//        inactiveEnemies.push_back(b);
//    }

    spawnTimer = 0.f;
    spawnedSoFar = 0;
    config.enemyCount += config.enemyCount;

    std::string jsonPath = "../../wave composition.json";
    int waveNumber = 1;

    std::ifstream file(jsonPath);
    if (!file.is_open())
        throw std::runtime_error("Could not open JSON file: " + jsonPath);

    json j; file >> j;
    std::string key = "wave " + std::to_string(waveNumber);
    if (!j.contains(key))
        throw std::runtime_error(key + " not found in JSON");
    auto &arr = j.at(key);


    for (auto &entry : arr) {
        std::string id = entry.at("id").get<std::string>();
        std::string type = entry.at("type").get<std::string>();

//        EnemyEntity* e;
        spawnConditions e;
        if (type == "ghoul")
            e.target = new Ghoul("../../images/support.png", paths[config.pathIndex], {rm.getGridData().squareWidth, rm.getGridData().squareHeight});
        else
            e.target = new Bat("../../images/support.png", paths[config.pathIndex], {rm.getGridData().squareWidth, rm.getGridData().squareHeight});

        e.target->setSpeed(entry.at("speed").get<float>());
        e.target->setKillReward(entry.at("killReward").get<int>());
        e.target->setHealth(entry.at("health").get<int>());
        e.target->setID(id);


        auto spawnArr = entry.at("spawn");
        std::string str;
        float delay = spawnArr.at(0).get<float>();
        Events ev = static_cast<Events>(spawnArr.at(1).get<int>());
        if (!spawnArr.at(2).is_null()) {
            str = spawnArr.at(2).get<std::string>();
        }

        e.time = delay;
        e.event = ev;
        e.id = str;

        inactiveEnemies.push_back(e.target);

        if (ev == Events::ON_DEATH) {
            idMap_[str] = e;
        } else {
            spawnQueue_.push(e);
        }
    }
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
//            spawnConditions temp = idMap_.find(e->getID());
            // check if any in spawn condition
            delete e;
            it = activeEnemies.erase(it);
        } else {
            ++it;
        }



    }

}





//void EnemyWave::spawnNext() {
//    if (spawnedSoFar >= config.enemyCount || inactiveEnemies.empty()) {
//        return;
//    }
//    EnemyEntity* e = inactiveEnemies.back();
//    inactiveEnemies.pop_back();
//
//    e->reset();
//    activeEnemies.push_back(e);
//    ++spawnedSoFar;
//}



//void EnemyWave::update(float dt) {
//    spawnTimer += dt;
//    if (spawnTimer >= config.spawnInterval) {
//        spawnNext();
//        spawnTimer = 0.f;
//    }
//
//    for (auto it = activeEnemies.begin(); it != activeEnemies.end();) {
//        EnemyEntity* e = *it;
//        e->update(dt);
//
//
//        if (!e->checkIfAlive()) {
//            // check if any in spawn condition
//            delete e;
//            it = activeEnemies.erase(it);
//        } else {
//            ++it;
//        }
//    }
//}


bool EnemyWave::isFinished() const {
    return spawnedSoFar >= config.enemyCount && activeEnemies.empty();
}


int EnemyWave::getRemainingCount() const {
    return (config.enemyCount - spawnedSoFar) + activeEnemies.size();
}


const std::vector<EnemyEntity*>& EnemyWave::getActiveEnemies() const {
    return activeEnemies;
}
