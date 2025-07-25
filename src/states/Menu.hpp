#pragma once
#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

class GameApp;

class Menu {
protected:
    GameApp* game = nullptr;

public:
    virtual ~Menu() = default;
    void setGame(GameApp* g) { game = g; }
    virtual bool handleUserInput(const sf::Event& event) = 0;
    virtual void menuActionUpdate(float delta) = 0;
    virtual void resize(sf::Vector2f scale) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};
