//
// Created by yareach on 26. 6. 3..
//

#include "auth_manager/auth/data/KeysInfo.h"

namespace auth_manager::auth {
    KeysInfo::KeysInfo(std::string created_at): _created_at(std::move(created_at)) {};

    KeysInfo KeysInfo::from_json(const nlohmann::json &json) {
        const std::string created_at = json["created_at"];
        return KeysInfo(created_at);
    }

    nlohmann::json KeysInfo::to_json() const {
        nlohmann::json json;
        json["created_at"] = _created_at;
        return json;
    }

    std::string KeysInfo::created_at() const {
        return _created_at;
    }

    void KeysInfo::set_created_at(const std::string &created_at) {
        _created_at = created_at;
    }
}
