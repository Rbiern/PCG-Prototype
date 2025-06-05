#pragma once
#include "Menu.hpp"
#include "../core/GameApp.hpp"
#include "../ui/PlayerCard.hpp"
#include "../ui/TowerInfo.hpp"
#include "../ui/SelectionButton.hpp"


class Squad : public Menu {
public:
    Squad();
    ~Squad() override;
    bool handleUserInput(const sf::Event& event) override;
    void menuActionUpdate(float delta) override;
    void resize(sf::Vector2f scale) override;
    void render(sf::RenderWindow& window) override;

private:
    TowerInfo* info;
    PlayerCard* selectedTowerCard;
    PlayerCard* currentDeck[8];
    std::array<SelectionButton, 4> deckSelection;
    sf::Texture backgroundTexture;
    sf::Texture homeButtonTexture;
    sf::Texture editButtonTexture;
    sf::Texture redoButtonTexture;
    sf::Texture nextButtonTexture;
    sf::Texture previousButtonTexture;
    sf::Texture selectedFrameTexture;
    sf::Sprite* backgroundSprite;
    sf::Sprite* homeButtonSprite;
    sf::Sprite* editButtonSprite;
    sf::Sprite* redoButtonSprite;
    sf::Sprite* nextButtonSprite;
    sf::Sprite* previousButtonSprite;
    sf::Sprite* selectedFrameSprite;
    ResourceManager& rm;
};
