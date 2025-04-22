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

private:
    std::vector<sf::Vector2i> path;
    size_t currentIdx = 0;
    sf::Vector2f tileSize;
    sf::Vector2f tileToWorld(const sf::Vector2i& t) const;
};
