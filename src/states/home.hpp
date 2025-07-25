#pragma once
#include "Menu.hpp"
#include "../core/GameApp.hpp"


class Home : public Menu {
public:
    Home();
    ~Home() override;
    bool handleUserInput(const sf::Event& event) override;
    void menuActionUpdate(float delta) override;
    void resize(sf::Vector2f scale) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Texture backgroundTexture;
    sf::Texture exitButtonTexture;
    sf::Texture settingsButtonTexture;
    sf::Texture squadButtonTexture;
    sf::Texture creatorButtonTexture;
    sf::Texture playButtonTexture;
    sf::Sprite* backgroundSprite;
    sf::Sprite* exitButtonSprite;
    sf::Sprite* settingsButtonSprite;
    sf::Sprite* squadButtonSprite;
    sf::Sprite* creatorButtonSprite;
    sf::Sprite* playButtonSprite;
    ResourceManager& rm;
};
