//
// Created by yareach on 26. 5. 31..
//

#ifndef AUTH_MANAGER_ROOT_KEY_MANAGER_OPENSSL_IMPL_H
#define AUTH_MANAGER_ROOT_KEY_MANAGER_OPENSSL_IMPL_H

#include "IRootKeyManager.h"
#include <openssl/evp.h>

namespace auth_manager::auth {
    class RootKeyManagerOpenSSLImpl : public IRootKeyManager {
    private:
        bool _is_key_loaded;

        EVP_PKEY *_private_key = nullptr;
        EVP_PKEY *_public_key = nullptr;

        static void extract_keys(const EVP_PKEY *pkey);

        void free_keys();
    public:
        RootKeyManagerOpenSSLImpl();

        ~RootKeyManagerOpenSSLImpl() override;

        void generate_new_keys() override;

        void load_keys() override;

        void update_keys() override;

        void delete_keys() override;

        bool is_key_loaded() override;
    };
}

#endif //AUTH_MANAGER_ROOT_KEY_MANAGER_OPENSSL_IMPL_H
