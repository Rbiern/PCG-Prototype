#include "TowerEntity.hpp"


class Tower : public TowerEntity {
public:
    explicit Tower(const std::string& id);
    ~Tower() override;
    bool OnClick(sf::Vector2i click) override;
    void setUserInteraction(bool status) override;
    bool isSelected() const override;
    void setPosition(const sf::Vector2f& pos) override;
    void setScale(sf::Vector2f scale) override;
    void setGridCoordinates(sf::Vector2i pos) override;
    sf::Vector2i getGridCoordinates() override;
    void setDirection(Direction newDir) override;
    Direction getDirection() override;
    void computeRange(const sf::Vector2i& origin, Direction dir, Tile* grid[16][24]) override;
    void update(float deltaTime, const std::vector<EnemyEntity*>& enemies) override;
    void render(sf::RenderWindow& window) override;

private:
    static std::vector<sf::Vector2i> rotateRange(const std::vector<sf::Vector2i>& offsets, Direction dir);
};
