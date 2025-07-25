#pragma once
#include <SFML/Graphics.hpp>
#include "../core/ResourceManager.hpp"


class SidePanel : public sf::Drawable {
    friend class Level;
public:
    SidePanel();
    ~SidePanel() override;
    void setScale(sf::Vector2f scale);
    void setRoundNumber(int stage);
    void setGoldNumber(int gold);
    void setHealthNumber(int health);
    void setRemainingNumber(int count);

private:
    void draw(sf::RenderTarget& rt, sf::RenderStates) const override;

    bool isVisible;
    sf::Vector2f basePosition;
    sf::RectangleShape panel;
    sf::RectangleShape divider;
    sf::Texture pauseButtonTexture;
    sf::Texture homeButtonTexture;
    sf::Texture abilitiesButtonTexture;
    sf::Texture coinsTexture;
    sf::Texture heartTexture;
    sf::Texture remainingEnemiesTexture;
    sf::Texture frameTexture;
    sf::Sprite* pauseButtonSprite;
    sf::Sprite* homeButtonSprite;
    sf::Sprite* abilitiesButtonSprite;
    sf::Sprite* coinsSprite;
    sf::Sprite* heartSprite;
    sf::Sprite* remainingEnemiesSprite;
    sf::Sprite* frameSprite;
    sf::Text* roundText;
    sf::Text* goldText;
    sf::Text* healthText;
    sf::Text* remainingText;
    sf::RectangleShape towerControlPanel;
    sf::RectangleShape towerControlDivider;
    sf::Texture xButtonTexture;
    sf::Texture towerFrameTexture;
    sf::Texture selectedTowerTexture;
    sf::Texture leftArrowButtonTexture;
    sf::Texture rightArrowButtonTexture;
    sf::Texture directionIconTexture;
    sf::Texture northButtonTexture;
    sf::Texture southButtonTexture;
    sf::Texture westButtonTexture;
    sf::Texture eastButtonTexture;
    sf::Texture sellButtonTexture;
    sf::Sprite* xButtonSprite;
    sf::Sprite* towerFrameSprite;
    sf::Sprite* selectedTowerSprite;
    sf::Sprite* leftArrowButtonSprite;
    sf::Sprite* rightArrowButtonSprite;
    sf::Sprite* directionIconSprite;
    sf::Sprite* northButtonSprite;
    sf::Sprite* southButtonSprite;
    sf::Sprite* westButtonSprite;
    sf::Sprite* eastButtonSprite;
    sf::Sprite* sellButtonSprite;
    sf::Text* towerNameText;
    sf::Text* killCountText;
    sf::Text* targetingText;
    sf::Text* directionText;
    sf::Text* sellTowerText;
};
