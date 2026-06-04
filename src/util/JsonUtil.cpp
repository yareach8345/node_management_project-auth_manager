//
// Created by yareach on 26. 6. 4..
//

#include "auth_manager/util/JsonUtil.h"
#include <fstream>
#include <iostream>

namespace auth_manager::util {
    void JsonUtil::save_json_file(const std::string &path, const nlohmann::json &json) {
        std::ofstream file(path);
        if (!file.is_open()) {
            std::cerr << "JsonUtil::load_json_file failed: " << path << std::endl;
            throw std::runtime_error("JsonUtil::load_json_file failed");
        }
        file << json.dump(4);
        file.close();
    }

    nlohmann::json JsonUtil::load_json_file(const std::string &path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "JsonUtil::load_json_file failed: " << path << std::endl;
            throw std::runtime_error("JsonUtil::load_json_file failed");
        }
        nlohmann::json json = nlohmann::json::parse(file);
        file.close();
        return json;
    }
}