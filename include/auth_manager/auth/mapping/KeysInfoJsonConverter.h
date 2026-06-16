//
// Created by yareach on 26. 6. 8..
//

#ifndef AUTH_MANAGER_KEYS_INFO_JSON_CONVERTER_H
#define AUTH_MANAGER_KEYS_INFO_JSON_CONVERTER_H

#include "auth_manager/core/json/JsonConverter.h"
#include "auth_manager/auth/data/KeysInfo.h"
#include <memory>

namespace auth_manager::auth {
    class KeysInfoJsonConverter: public core::json::JsonConverter<KeysInfo> {
    private:
        static std::shared_ptr<KeysInfoJsonConverter> _instance;

        [[nodiscard]] nlohmann::json to_json(const KeysInfo &data) const override;

        [[nodiscard]] KeysInfo from_json(const nlohmann::json &json) const override;
    public:
        static std::shared_ptr<JsonConverter<KeysInfo>> get_instance();
    };
}

#endif //AUTH_MANAGER_KEYS_INFO_JSON_CONVERTER_H
