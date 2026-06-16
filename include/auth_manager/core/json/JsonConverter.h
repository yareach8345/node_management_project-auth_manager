//
// Created by yareach on 26. 6. 8..
//

#ifndef AUTH_MANAGER_JSON_CONVERTER_H
#define AUTH_MANAGER_JSON_CONVERTER_H

#include <string>
#include <nlohmann/json.hpp>

namespace auth_manager::core::json {
    template<typename T>
    class JsonConverter {
    private:
        [[nodiscard]] virtual nlohmann::json to_json(const T& data) const = 0;

        [[nodiscard]] virtual T from_json(const nlohmann::json& json) const = 0;
    public:
        virtual ~JsonConverter() = default;

        [[nodiscard]] std::string serialize(const T& data, int indent) const;

        [[nodiscard]] std::string serialize(const T& data) const;

        [[nodiscard]] T deserialize(std::string_view json_string) const;
    };

    template<typename T>
    std::string JsonConverter<T>::serialize(const T& data, const int indent) const {
        return to_json(data).dump(indent);
    }

    template<typename T>
    std::string JsonConverter<T>::serialize(const T &data) const {
        return to_json(data).dump();
    }

    template<typename T>
    T JsonConverter<T>::deserialize(const std::string_view json_string) const {
        const auto json = nlohmann::json::parse(json_string);
        return from_json(json);
    }
}

#endif //AUTH_MANAGER_JSON_CONVERTER_H
