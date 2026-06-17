//
// Created by yareach on 26. 6. 7..
//

#ifndef AUTH_MANAGER_KEY_SERVICE_IMPL_H
#define AUTH_MANAGER_KEY_SERVICE_IMPL_H

#include "IKeyService.h"

#include "auth_manager/auth/config/AuthConfig.h"
#include "auth_manager/auth/key_provider/KeyProvider.h"
#include "auth_manager/auth/mapping/KeysInfoJsonConverter.h"
#include "auth_manager/core/json/JsonFileManager.h"

namespace auth_manager::auth {
    class KeyServiceImpl : public IKeyService {
    private:
        std::shared_ptr<key_provider::KeyProvider> _key_provider;

        core::json::JsonFileManager<KeysInfo> _json_file_manager;

        std::string _keys_info_file_path;
        std::optional<KeysInfo> _keys_info;

        [[nodiscard]] std::array<std::string_view, 3> required_files() const;
    public:
        explicit KeyServiceImpl(
            std::shared_ptr<key_provider::KeyProvider> key_provider,
            const AuthConfig &auth_config,
            const std::string& key_name
        );

        ~KeyServiceImpl() override;

        void generate_new_keys() override;
        void load_keys() override;
        void update_keys() override;
        void delete_keys() override;

        std::vector<std::byte> sign(const std::string &message) override;

        bool verify(const std::string& message, const std::vector<std::byte>& signature) override;

        [[nodiscard]] std::string_view key_name() const override;

        [[nodiscard]] bool is_key_loaded() const override;

        [[nodiscard]] std::string_view private_key_file_path() const override;
        [[nodiscard]] std::string_view public_key_file_path() const override;
        [[nodiscard]] std::string export_public_key() const override;

        [[nodiscard]] std::string_view keys_info_file_path() const override;
        [[nodiscard]] std::optional<KeysInfo> keys_info() const override;
    };
}

#endif //AUTH_MANAGER_KEY_SERVICE_IMPL_H
