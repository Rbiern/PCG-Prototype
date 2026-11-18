#pragma once

#include "core/ApplicationSettings.hpp"
#include "engine/assets/ResourceManager.hpp"
#include "engine/audio/SoundEngine.hpp"
#include "engine/time/TimeCalculator.hpp"
#include "core/Window.hpp"
#include "states/Screen.hpp"
#include "states/ScreenManager.h"


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
    SoundEngine soundEngine;
    TimeCalculator timeCalculator;
    Window window;
    ScreenManager screenManager;
    bool closeRequested = false;
};
