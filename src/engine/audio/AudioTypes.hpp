#pragma once

#include <SFML/System/Vector2.hpp>
#include <cstdint>
#include <optional>


enum class AudioCategory {
    UI,
    SFX
};


enum class AudioLifetime {
    Scope,
    Application
};


// IDs are never reused. A zero ID means unbound/invalid.
struct AudioScopeId {
    std::uint64_t value = 0;
};


struct SoundHandle {
    std::uint64_t value = 0;

    [[nodiscard]] bool isValid() const {
        return value != 0;
    }
};


struct SoundPlaybackOptions {
    std::optional<sf::Vector2f> position;
    std::optional<bool> looping;
    float gain = 1.f;
};
