#pragma once
#include "Menu.hpp"
#include "../core/Game.hpp"
#include "../ui/SidePanel.hpp"
#include "../ui/PlayerCard.hpp"
#include "../levels/GameField.hpp"
#include "../Generator.hpp"
#include "../Tile.hpp"
#include "../GrassTile.cpp"


class Level : public Menu {
public:
    Level();
    ~Level() override;
    bool handleUserInput(const sf::Event& event) override;
    void menuActionUpdate() override;
    void render(sf::RenderWindow& window) override;

private:
    // Set up 85% of the content left of the screen
    sf::Font font;
    sf::Text* towerCountText;
    sf::Text* remainingText;
    sf::RectangleShape divider;
    sf::Texture pauseButton;
    sf::Texture towerCountIcon;
    sf::Texture remainingEnemiesIcon;
    sf::Sprite* pauseButtonSprite = nullptr;
    sf::Sprite* towerCountIconSprite = nullptr;
    sf::Sprite* remainingEnemiesIconSprite = nullptr;

    // Set up the panel on the right side of the screen. Panel takes up the remaining 15% of the right side of screen
    SidePanel panel;
    PlayerCard* towerSelection[8];

    bool pauseFlag;
    int playerTowerCount;
    int gold;
    int remainingEnemies;
    int hp;
    int waveCount;
    sf::Clock time;

//    Generator* PCG;
    GameField gameField;

    ResourceManager& rm;
};
