//
// Created by yareach on 26. 6. 8..
//

#include "auth_manager/auth/mapping/KeysInfoJsonConverter.h"
#include <nlohmann/json.hpp>

namespace auth_manager::auth {
    std::shared_ptr<KeysInfoJsonConverter> KeysInfoJsonConverter::_instance = nullptr;

    std::shared_ptr<core::json::IJsonConverter<KeysInfo>> KeysInfoJsonConverter::get_instance() {
        if (!_instance) { _instance = std::make_shared<KeysInfoJsonConverter>(); }
        return _instance;
    }

    std::string KeysInfoJsonConverter::serialize(const KeysInfo &data, const int indent) const {
        const nlohmann::json json = {{ "created_at", data.created_at }};
        return json.dump(indent);
    }

    KeysInfo KeysInfoJsonConverter::deserialize(std::string_view json_string) const {
        const nlohmann::json json = nlohmann::json::parse(json_string);
        const std::string created_at = json["created_at"];
        return { .created_at = created_at };
    }
}
