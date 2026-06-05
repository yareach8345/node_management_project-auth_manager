//
// Created by yareach on 26. 5. 31..
//

#ifndef AUTH_MANAGER_ROOT_KEY_MANAGER_OPENSSL_IMPL_H
#define AUTH_MANAGER_ROOT_KEY_MANAGER_OPENSSL_IMPL_H

#include "IRootKeyManager.h"
#include "auth_manager/auth/config/AuthConfig.h"
#include "RootKeysInfo.h"
#include <openssl/evp.h>
#include <optional>

namespace auth_manager::auth {
    class RootKeyManagerOpenSSLImpl : public IRootKeyManager<const EVP_PKEY*, const EVP_PKEY*> {
    private:
        struct EVP_PKEY_Deleter {
            void operator()(EVP_PKEY* p) const;
        };
        using EVPKeyPointer = std::unique_ptr<EVP_PKEY, EVP_PKEY_Deleter>;

        const std::string _private_key_file_path;
        const std::string _public_key_file_path;
        const std::string _keys_info_file_path;

        EVPKeyPointer _private_key = nullptr;
        EVPKeyPointer _public_key = nullptr;

        std::optional<RootKeysInfo> _root_keys_info;

        void extract_keys(const EVP_PKEY *pkey) const;
        void clear();
    public:
        explicit RootKeyManagerOpenSSLImpl(const AuthConfig &auth_config);

        ~RootKeyManagerOpenSSLImpl() override;

        void generate_new_keys() override;
        void load_keys() override;
        void update_keys() override;
        void delete_keys() override;
        [[nodiscard]] bool is_key_loaded() const override;

        [[nodiscard]] std::string_view private_key_file_path() const override;
        [[nodiscard]] const EVP_PKEY* private_key() const override;

        [[nodiscard]] std::string_view public_key_file_path() const override;
        [[nodiscard]] const EVP_PKEY* public_key() const override;

        [[nodiscard]] std::string_view keys_info_file_path() const override;
        [[nodiscard]] const std::optional<RootKeysInfo>& root_keys_info() const override;
    };
}

#endif //AUTH_MANAGER_ROOT_KEY_MANAGER_OPENSSL_IMPL_H
