//
// Created by yareach on 26. 6. 3..
//

#ifndef AUTH_MANAGER_KEYS_INFO_H
#define AUTH_MANAGER_KEYS_INFO_H

#include <string>
#include <nlohmann/json.hpp>

namespace auth_manager::auth {
    class KeysInfo {
    private:
        std::string _created_at;
    public:
        explicit KeysInfo(std::string created_at);

        static KeysInfo from_json(const nlohmann::json &json);

        [[nodiscard]] nlohmann::json to_json() const;

        [[nodiscard]] std::string created_at() const;

        void set_created_at(const std::string &created_at);
    };
}

#endif //AUTH_MANAGER_KEYS_INFO_H
