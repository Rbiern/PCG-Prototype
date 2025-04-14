#pragma once
#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>


class Game;

// Abstract class
class Menu {
protected:
    Game* game = nullptr;

public:
    virtual ~Menu() = default;
    void setGame(Game* g) { game = g; }
    virtual bool handleUserInput(const sf::Event& event) = 0;
    virtual void menuActionUpdate() = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};
