#pragma once

#include <functional>
#include <SFML/Graphics.hpp>
#include "engine/time/TimeCalculator.hpp"


class ResourceManager;
class ScreenManager;
class GameApp;

struct ScreenContext {
    ResourceManager& resources;
    ScreenManager& screens;

    std::function<void()> closeApplication;
    std::function<void()> toggleFullscreen;
};

class Screen {
public:
    explicit Screen(ScreenContext screenContext) : context(std::move(screenContext)) {}
    virtual ~Screen() = default;
    Screen(const Screen&) = delete;
    Screen& operator=(const Screen&) = delete;

    virtual void handleEvent(const sf::Event& event, sf::Vector2f mousePos) = 0;
    virtual void update(const FrameTime& time) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

protected:
    ScreenContext context;

private:
    friend class GameApp;
};
