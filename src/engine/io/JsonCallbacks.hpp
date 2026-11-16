#pragma once

#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>
#include "infrastructure/json.hpp"


namespace JsonCallbacks {
    inline constexpr int MaxDepth = 8;

    // Create a fresh callback for each parse; key tracking belongs to that parse.
    // Throws runtime_error for duplicate keys or excessive parser-event depth.
    [[nodiscard]] inline nlohmann::json::parser_callback_t makeValidationCallback() {
        return [objectKeys = std::vector<std::unordered_set<std::string>>{}](
            int depth,
            nlohmann::json::parse_event_t event,
            nlohmann::json& value) mutable -> bool {

            if (depth > MaxDepth) {
                throw std::runtime_error("JSON nesting exceeds " + std::to_string(MaxDepth) + " levels");
            }

            if (event == nlohmann::json::parse_event_t::object_start) {
                objectKeys.emplace_back();
            } else if (event == nlohmann::json::parse_event_t::key) {
                const std::string key = value.get<std::string>();

                if (!objectKeys.back().insert(key).second) {
                    throw std::runtime_error("Duplicate JSON key: " + key);
                }
            } else if (event == nlohmann::json::parse_event_t::object_end) {
                objectKeys.pop_back();
            }

            return true;
        };
    }
}
