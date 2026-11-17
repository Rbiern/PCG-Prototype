#pragma once

#include "AudioTypes.hpp"
#include <filesystem>
#include <string>
#include <unordered_map>


struct SoundDefinition {
    std::filesystem::path file;
    AudioCategory category = AudioCategory::SFX;
    AudioLifetime lifetime = AudioLifetime::Scope;
    float volume = 1.f;             // volume multiplier, from 0 to 1
    bool loop = false;
    bool spatial = false;
    float minDistance = 192.f;      // World units; 64 world units = one audio meter.
    float attenuation = 1.f;        // 0 disables distance attenuation
    unsigned int maxInstances = 8;
};


struct MusicDefinition {
    std::filesystem::path file;
    AudioLifetime lifetime = AudioLifetime::Scope;
    float volume = 1.f;
    bool loop = true;
};


struct AudioCatalog {
    std::unordered_map<std::string, SoundDefinition> sounds;
    std::unordered_map<std::string, MusicDefinition> music;
};
