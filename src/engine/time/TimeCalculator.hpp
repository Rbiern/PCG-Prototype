#pragma once

#include <SFML/System/Clock.hpp>


struct FrameTime {
    float uiDelta = 0.f;
    float gameDelta = 0.f;
    double realElapsed = 0.0;
};


class TimeCalculator {
public:
    [[nodiscard]] FrameTime tick();

    void setPaused(bool paused) noexcept;
    [[nodiscard]] bool isPaused() const noexcept;

    void setTimeScale(float scale);
    [[nodiscard]] float getTimeScale() const noexcept;

private:
    sf::Clock clock;
    double rawDeltaTime = 0.0;
    float timeScale = 1.f;
    bool paused = false;
};
