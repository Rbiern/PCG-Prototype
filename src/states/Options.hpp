#pragma once
#include "Menu.hpp"
#include "../core/GameApp.hpp"
#include "../ui/Slider.hpp"
#include "../ui/SoundBar.hpp"


class Options : public Menu {
public:
    Options();
    ~Options() override;
    bool handleUserInput(const sf::Event& event) override;
    void menuActionUpdate(float delta) override;
    void resize(sf::Vector2f scale) override;
    void render(sf::RenderWindow& window) override;

private:
    bool videoFlag;
    bool videoFrameBox;
    bool volumeFlag;
    bool profileFlag;
    windowConfig* config;
    sf::Texture backgroundTexture;
    sf::Texture homeButtonTexture;
    sf::Texture applyButtonTexture;
    sf::Texture selectedBarTexture;
    sf::Texture switchTexture;
    sf::Texture leftArrowTexture;
    sf::Texture rightArrowTexture;
    sf::Texture frameBoxTexture;
    sf::Texture textBoxTexture;
    // BASE Settings
    sf::Sprite* backgroundSprite;
    sf::Sprite* homeButtonSprite;
    sf::Sprite* applyButtonSprite;
    sf::Sprite* selectedHitBoxSprite1;
    sf::Sprite* selectedHitBoxSprite2;
    sf::Sprite* selectedHitBoxSprite3;
    sf::Text* videoText;
    sf::Text* volumeText;
    sf::Text* profileText;
    // VIDEO SECTION
    Slider* slider1;
    Slider* slider2;
    sf::Sprite* leftArrowSprite;
    sf::Sprite* rightArrowSprite;
    sf::Sprite* switchButtonSprite1;
    sf::Sprite* switchButtonSprite2;
    sf::Sprite* switchButtonSprite3;
    sf::Sprite* boxSprite;
    sf::Text* fullscreenText;
    sf::Text* resolutionText;
    sf::Text* resolutionRatio;
    sf::Text* antialiasingText;
    sf::Text* frameRateText;
    sf::Text* verticalSyncText;
    // VOLUME SECTION
    SoundBar* musicBar;
    SoundBar* soundBar;
    sf::Sprite* switchButtonSprite4;
    sf::Sprite* switchButtonSprite5;
    sf::Text* musicText;
    sf::Text* soundEffectText;
    // PROFILE SECTION
    sf::Sprite* textBoxSprite1;
    sf::Sprite* textBoxSprite2;
    sf::Text* statisticsText;
    sf::Text* accountText;
    // Other
    ResourceManager& rm;
};
