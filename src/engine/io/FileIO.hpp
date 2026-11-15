#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <filesystem>


namespace FileIO {
    inline constexpr std::size_t OneMebibyte = 1024 * 1024;

    [[nodiscard]] bool readFileOneShot(
        const std::filesystem::path& inputPath,
        std::string& outPayload,
        std::size_t maxBytes,
        std::string& error);

    [[nodiscard]] bool writeFileAtomically(
        const std::filesystem::path& path,
        std::string_view contents,
        std::string& error);
}
