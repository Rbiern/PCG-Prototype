#pragma once

#include "core/ApplicationSettings.hpp"
#include "engine/time/TimeCalculator.hpp"
#include "engine/assets/ResourceManager.hpp"
#include "states/Screen.hpp"
#include "states/ScreenManager.h"
#include "core/Window.hpp"


class GameApp final {
public:
    explicit GameApp();
    ~GameApp();
    GameApp(const GameApp&) = delete;
    GameApp& operator=(const GameApp&) = delete;

    void toggleFullscreen();
    void closeApp();
    int execute();

private:
    ScreenContext makeScreenContext();

    ApplicationSettings appSettings;
    ResourceManager resourceManager;
    TimeCalculator timeCalculator;
    ScreenManager screenManager;
    Window window;
    bool closeRequested = false;
};
