//
// Created by yareach on 26. 6. 26..
//

#include "auth_manager/auth/service/KeyServiceImpl.h"
#include "auth_manager/auth/factory/key_provider_factory.h"

namespace auth_manager::root_key {
    using namespace auth_manager::auth;

    IKeyService* generate_root_key_service(const AuthConfig& auth_config) {
        std::unique_ptr<KeyProvider> provider(generate_key_provider(auth_config.file_base(), "root"));
        const auto json_file_manager = core::json::JsonFileManager(auth_config.file_base() + "/root/keys_info.json", KeysInfoJsonConverter::get_instance());

        return new KeyServiceImpl(std::move(provider), json_file_manager);
    }
}