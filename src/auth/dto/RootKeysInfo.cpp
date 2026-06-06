//
// Created by yareach on 26. 6. 3..
//

#include "auth_manager/auth/dto/RootKeysInfo.h"

namespace auth_manager::auth {
    RootKeysInfo::RootKeysInfo(std::string created_at): _created_at(std::move(created_at)) {};

    RootKeysInfo RootKeysInfo::from_json(const nlohmann::json &json) {
        const std::string created_at = json["created_at"];
        return RootKeysInfo(created_at);
    }

    nlohmann::json RootKeysInfo::to_json() const {
        nlohmann::json json;
        json["created_at"] = _created_at;
        return json;
    }

    std::string RootKeysInfo::created_at() const {
        return _created_at;
    }

    void RootKeysInfo::set_created_at(const std::string &created_at) {
        _created_at = created_at;
    }
}
