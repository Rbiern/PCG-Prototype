#include "EnemyEntity.hpp"


class Ghoul: public EnemyEntity {
public:
    Ghoul(const std::string& image, const std::vector<sf::Vector2i>& pathTiles, const sf::Vector2f& cellSize);
    ~Ghoul() override;
    void takeDamage(int dmg) override;
    float getDistanceTraveled() const override;
    void setPosition(const sf::Vector2f& pos) override;
    sf::Vector2f getPosition() override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
    bool hasFinishedPath() const;
    void setKillReward(int gold) override;
    void setHealth(int hp) override;
    int getMaxHealth() const override;
    void setMaxHealth(int maxHP) override;
    void setSpeed(float speed) override;
    float getSpeed() const override;
    bool checkIfAlive() override;
    void reset() override;

private:
    std::vector<sf::Vector2i> path;
    size_t currentIdx = 0;
    sf::Vector2f tileSize;
    sf::Vector2f tileToWorld(const sf::Vector2i& t) const;
};
