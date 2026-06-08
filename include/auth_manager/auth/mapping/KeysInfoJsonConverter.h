//
// Created by yareach on 26. 6. 8..
//

#ifndef AUTH_MANAGER_KEYS_INFO_JSON_CONVERTER_H
#define AUTH_MANAGER_KEYS_INFO_JSON_CONVERTER_H

#include "auth_manager/core/json/IJsonConverter.h"
#include "auth_manager/auth/data/KeysInfo.h"
#include <memory>

namespace auth_manager::auth {
    class KeysInfoJsonConverter: public core::json::IJsonConverter<KeysInfo> {
    private:
        static std::shared_ptr<KeysInfoJsonConverter> _instance;
    public:
        static std::shared_ptr<IJsonConverter<KeysInfo>> get_instance();

        std::string serialize(const KeysInfo &data, int indent) const override;

        KeysInfo deserialize(std::string_view json_string) const override;
    };
}

#endif //AUTH_MANAGER_KEYS_INFO_JSON_CONVERTER_H
