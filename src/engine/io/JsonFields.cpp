#include "JsonFields.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>


namespace JsonFields {
    FieldResult<bool> readBoolean(const nlohmann::json& object, std::string_view key) {
        if (!object.is_object()) {
            throw std::invalid_argument("JSON field readers require an object");
        }

        const auto it = object.find(std::string(key));
        const auto* value = it == object.end() ? nullptr : &*it;

        if (!value) {
            return {std::nullopt, FieldStatus::Missing};
        }
        if (!value->is_boolean()) {
            return {std::nullopt, FieldStatus::Invalid};
        }
        return {value->get<bool>(), FieldStatus::Valid};
    }


    FieldResult<std::string> readString(const nlohmann::json& object, std::string_view key) {
        if (!object.is_object()) {
            throw std::invalid_argument("JSON field readers require an object");
        }

        const auto it = object.find(std::string(key));
        const auto* value = it == object.end() ? nullptr : &*it;

        if (!value) {
            return {std::nullopt, FieldStatus::Missing};
        }
        if (!value->is_string()) {
            return {std::nullopt, FieldStatus::Invalid};
        }
        return {value->get<std::string>(), FieldStatus::Valid};
    }


    FieldResult<std::uint32_t> readUInt32(const nlohmann::json& object, std::string_view key, std::uint32_t minimum, std::uint32_t maximum) {
        if (minimum > maximum) {
            throw std::invalid_argument("Invalid integer bounds");
        }

        if (!object.is_object()) {
            throw std::invalid_argument("JSON field readers require an object");
        }

        const auto it = object.find(std::string(key));
        const auto* value = it == object.end() ? nullptr : &*it;

        if (!value) {
            return {std::nullopt, FieldStatus::Missing};
        }
        if (value->is_number_unsigned()) {
            const auto original = value->get<std::uint64_t>();
            const auto bounded = std::clamp(original, static_cast<std::uint64_t>(minimum), static_cast<std::uint64_t>(maximum));
            return {static_cast<std::uint32_t>(bounded), original != bounded ? FieldStatus::Clamped : FieldStatus::Valid};
        }
        if (value->is_number_integer()) {
            const auto original = value->get<std::int64_t>();
            const auto bounded = std::clamp(original, static_cast<std::int64_t>(minimum), static_cast<std::int64_t>(maximum));
            return {static_cast<std::uint32_t>(bounded), original != bounded ? FieldStatus::Clamped : FieldStatus::Valid};
        }
        return {std::nullopt, FieldStatus::Invalid};
    }


    FieldResult<float> readFloat(const nlohmann::json& object, std::string_view key, float minimum, float maximum) {
        if (!std::isfinite(minimum) || !std::isfinite(maximum) || minimum > maximum) {
            throw std::invalid_argument("Invalid floating-point bounds");
        }

        if (!object.is_object()) {
            throw std::invalid_argument("JSON field readers require an object");
        }

        const auto it = object.find(std::string(key));
        const auto* value = it == object.end() ? nullptr : &*it;

        if (!value) {
            return {std::nullopt, FieldStatus::Missing};
        }
        if (!value->is_number()) {
            return {std::nullopt, FieldStatus::Invalid};
        }

        const double original = value->get<double>();
        if (!std::isfinite(original)) {
            return {std::nullopt, FieldStatus::Invalid};
        }

        const double bounded = std::clamp(original,static_cast<double>(minimum), static_cast<double>(maximum));
        return {static_cast<float>(bounded), original != bounded ? FieldStatus::Clamped : FieldStatus::Valid};
    }


    FieldResult<std::string> readChoice(const nlohmann::json& object, std::string_view key, std::initializer_list<std::string_view> allowed) {
        auto result = readString(object, key);
        if (!result.value) {
            return result;
        }

        const std::string_view text = *result.value;
        if (std::find(allowed.begin(), allowed.end(), text) == allowed.end()) {
            return {std::nullopt, FieldStatus::Invalid};
        }
        return result;
    }


    FieldResult<const nlohmann::json*> readObject(const nlohmann::json& object, std::string_view key) {
        if (!object.is_object()) {
            throw std::invalid_argument("JSON field readers require an object");
        }

        const auto it = object.find(std::string(key));
        const auto* value = it == object.end() ? nullptr : &*it;

        if (!value) {
            return {std::nullopt, FieldStatus::Missing};
        }
        if (!value->is_object()) {
            return {std::nullopt, FieldStatus::Invalid};
        }
        return {value, FieldStatus::Valid};
    }
}
