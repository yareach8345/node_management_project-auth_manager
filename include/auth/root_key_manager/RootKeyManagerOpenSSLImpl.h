//
// Created by yareach on 26. 5. 31..
//

#ifndef AUTH_MANAGER_ROOT_KEY_MANAGER_OPENSSL_IMPL_H
#define AUTH_MANAGER_ROOT_KEY_MANAGER_OPENSSL_IMPL_H

#include "IRootKeyManager.h"
#include "auth/config/AuthConfig.h"
#include <openssl/evp.h>

namespace auth_manager::auth {
    class RootKeyManagerOpenSSLImpl : public IRootKeyManager {
    private:
        const std::string _private_key_file_path;

        const std::string _public_key_file_path;

        bool _is_key_loaded;

        EVP_PKEY *_private_key = nullptr;
        EVP_PKEY *_public_key = nullptr;

        void extract_keys(const EVP_PKEY *pkey) const;

        void free_keys();
    public:
        explicit RootKeyManagerOpenSSLImpl(const AuthConfig &auth_config);

        ~RootKeyManagerOpenSSLImpl() override;

        void generate_new_keys() override;

        void load_keys() override;

        void update_keys() override;

        void delete_keys() override;

        bool is_key_loaded() override;

        const std::string& private_key_file_path() override;

        const std::string& public_key_file_path() override;
    };
}

#endif //AUTH_MANAGER_ROOT_KEY_MANAGER_OPENSSL_IMPL_H
