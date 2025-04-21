#pragma once
#include "Menu.hpp"
#include "../core/Game.hpp"
#include "../ui/LevelHUD.hpp"
#include "../ui/SidePanel.hpp"
#include "../ui/PlayerCard.hpp"
#include "../levels/GameField.hpp"
#include "../levels/tiles/Tile.hpp"
#include "../levels/tiles/GrassTile.cpp"
#include "../levels/tiles/WaterTile.cpp"


class Level : public Menu {
public:
    Level();
    ~Level() override;
    bool handleUserInput(const sf::Event& event) override;
    void menuActionUpdate(float delta) override;
    void render(sf::RenderWindow& window) override;

private:
    void setScale();
    // UI
    LevelHUD hud;       // Heads up display takes up 85% of the left of the screen
    SidePanel panel;    // panel takes up 15% of the right side of the screen
    // Game stuff
    PlayerCard* towerSelection[8];
    GameField gameField;
    // Input controls
    bool pauseFlag;
    bool exitFlag;
    bool isDragging;
    int currTowerSel;
    sf::Sprite* draggedSprite;
    // Other
    ResourceManager& rm;
};
