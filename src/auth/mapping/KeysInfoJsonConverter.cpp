//
// Created by yareach on 26. 6. 8..
//

#include "auth_manager/auth/mapping/KeysInfoJsonConverter.h"
#include <nlohmann/json.hpp>

namespace auth_manager::auth {
    std::shared_ptr<KeysInfoJsonConverter> KeysInfoJsonConverter::_instance = nullptr;

    std::shared_ptr<core::json::JsonConverter<KeysInfo>> KeysInfoJsonConverter::get_instance() {
        if (!_instance) { _instance = std::make_shared<KeysInfoJsonConverter>(); }
        return _instance;
    }

    nlohmann::json KeysInfoJsonConverter::to_json(const KeysInfo &data) const {
        return {
            { "created_at" , data.created_at },
            { "algorithm" , data.algorithm },
            { "bits", data.bits }
        };
    }

    KeysInfo KeysInfoJsonConverter::from_json(const nlohmann::json &json) const {
        return {
            .created_at = json["created_at"].get<std::string>(),
            .algorithm = json["algorithm"].get<std::string>(),
            .bits = json["bits"].get<int>()
        };
    }
}
