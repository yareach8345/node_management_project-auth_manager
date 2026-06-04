//
// Created by yareach on 26. 6. 4..
//

#include <nlohmann/json.hpp>

#ifndef AUTH_MANAGER_JSON_UTIl_H
#define AUTH_MANAGER_JSON_UTIl_H

namespace auth_manager::util {
    class JsonUtil {
    public:
        static void save_json_file(const std::string &path, const nlohmann::json &json);

        static nlohmann::json load_json_file(const std::string &path);
    };
}

#endif //AUTH_MANAGER_JSON_UTIl_H