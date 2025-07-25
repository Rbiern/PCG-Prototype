#pragma once
#include "Menu.hpp"
#include "../core/GameApp.hpp"


class Creator : public Menu {
public:
    Creator();
    ~Creator() override;
    bool handleUserInput(const sf::Event& event) override;
    void menuActionUpdate(float delta) override;
    void resize(sf::Vector2f scale) override;
    void render(sf::RenderWindow& window) override;

private:
    sf::Texture background;
    sf::Texture homeButton;

    sf::Sprite* backgroundSprite;
    sf::Sprite* homeButtonSprite;
    ResourceManager& rm;
};
