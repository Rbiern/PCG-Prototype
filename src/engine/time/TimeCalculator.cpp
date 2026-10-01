#include "TimeCalculator.hpp"

#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <SFML/System/Time.hpp>


namespace {
    constexpr double MaxFrameDelta = 0.05; // 50 ms
    constexpr float MaxTimeScale = 4.f;
}


FrameTime TimeCalculator::tick() {
    // 1 second = 1,000,000 microseconds
    const double rawDelta = static_cast<double>(clock.restart().asMicroseconds()) / 1'000'000.0;
    const double validDelta = std::isfinite(rawDelta) && rawDelta > 0.0 ? rawDelta : 0.0;

    rawDeltaTime += validDelta;

    const float uiDelta = static_cast<float>(std::min(validDelta, MaxFrameDelta));
    const float gameDelta = paused ? 0.f : uiDelta * timeScale;

    return {uiDelta, gameDelta, rawDeltaTime};
}


void TimeCalculator::setPaused(bool paused) noexcept {
    this->paused = paused;
}


bool TimeCalculator::isPaused() const noexcept {
    return paused;
}


void TimeCalculator::setTimeScale(float scale) {
    if (!std::isfinite(scale) || scale <= 0.f || scale > MaxTimeScale) {
        throw std::invalid_argument("Time scale must be greater than 0 and at most 4");
    }

    timeScale = scale;
}


float TimeCalculator::getTimeScale() const noexcept {
    return timeScale;
}
