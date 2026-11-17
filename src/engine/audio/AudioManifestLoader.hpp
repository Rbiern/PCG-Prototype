#pragma once

#include "AudioDefinitions.hpp"
#include <filesystem>
#include <string>


class AudioManifestLoader final {
public:
    explicit AudioManifestLoader(std::filesystem::path path);

    [[nodiscard]] bool load(std::string& error);
    [[nodiscard]] const AudioCatalog& catalog() const noexcept;

private:
    std::filesystem::path path;
    AudioCatalog audioCatalog;
};
