#pragma once

#include <vector>
#include <queue>
#include "core/ApplicationSettings.hpp"
#include "core/Window.hpp"
#include "engine/time/TimeCalculator.hpp"
#include "engine/assets/ResourceManager.hpp"


class Screen;

enum class TransitionType {Push, Pop, Switch, Close};
struct Transition {TransitionType type; Screen* state;};

class GameApp final {
public:
    explicit GameApp();
    ~GameApp();
    GameApp(const GameApp&) = delete;
    GameApp& operator=(const GameApp&) = delete;

    void pushState(Screen* state);
    void popState();
    void switchState(Screen* state);
    void close();

    ResourceManager& resources();
    void toggleFullScreen();
    int execute();

private:
    void stateTransition();

    ApplicationSettings appSettings;
    Window window;
    TimeCalculator timeCalculator;
    ResourceManager resourceManager;
    std::vector<Screen*> stateStack;
    std::queue<Transition> transitions;
};
