#pragma once
#include "Menu.hpp"
#include "../core/GameApp.hpp"


class StartUp : public Menu {
public:
    StartUp();
    ~StartUp() override;
    bool handleUserInput(const sf::Event& event) override;
    void menuActionUpdate(float delta) override;
    void resize(sf::Vector2f scale) override;
    void render(sf::RenderWindow& window) override;

private:
    bool flash;
    float flashTimer;
    sf::RectangleShape startButton;
    sf::Texture backgroundTexture;
    sf::Sprite* backgroundSprite;
    ResourceManager& rm;
};
