//
// Created by yareach on 26. 6. 8..
//

#ifndef AUTH_MANAGER_I_JSON_CONVERTER_H
#define AUTH_MANAGER_I_JSON_CONVERTER_H

#include <string>

namespace auth_manager::core::json {
    template<typename T>
    class IJsonConverter {
    public:
        virtual ~IJsonConverter() = default;

        virtual std::string serialize(const T& data, int indent) const = 0;

        std::string serialize(const T& data) const;

        virtual T deserialize(std::string_view json_string) const = 0;
    };

    template<typename T>
    std::string IJsonConverter<T>::serialize(const T &data) const {
        return serialize(data, 0);
    }
}

#endif //AUTH_MANAGER_I_JSON_CONVERTER_H
