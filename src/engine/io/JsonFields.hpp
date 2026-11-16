#pragma once

#include <cstdint>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include "infrastructure/json.hpp"


namespace JsonFields {
    enum class FieldStatus {
        Valid,
        Missing,
        Invalid,
        Clamped
    };

    template<class T>
    struct FieldResult {
        std::optional<T> value;
        FieldStatus status = FieldStatus::Missing;

        [[nodiscard]] T valueOr(T fallback, bool* repairNeeded = nullptr) const {
            if (repairNeeded && status != FieldStatus::Valid) {
                *repairNeeded = true;
            }
            return value.value_or(fallback);
        }
    };

    // Parents must be objects; otherwise these functions throw invalid_argument.
    // Missing and invalid fields have no value. JSON null is invalid, not missing.
    // Readers do not choose defaults, log errors, or change the document.
    [[nodiscard]] FieldResult<bool> readBoolean(
        const nlohmann::json& object, std::string_view key);

    [[nodiscard]] FieldResult<std::string> readString(
        const nlohmann::json& object, std::string_view key);

    [[nodiscard]] FieldResult<std::uint32_t> readUInt32(
        const nlohmann::json& object, std::string_view key,
        std::uint32_t minimum, std::uint32_t maximum);

    [[nodiscard]] FieldResult<float> readFloat(
        const nlohmann::json& object, std::string_view key,
        float minimum, float maximum);

    [[nodiscard]] FieldResult<std::string> readChoice(
        const nlohmann::json& object, std::string_view key,
        std::initializer_list<std::string_view> allowed);

    [[nodiscard]] FieldResult<const nlohmann::json*> readObject(
        const nlohmann::json& object, std::string_view key);
}
