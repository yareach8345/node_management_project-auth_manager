//
// Created by yareach on 26. 6. 10..
//

#ifndef AUTH_MANAGER_KEY_PROVIDER_OPENSSL_IMPL_H
#define AUTH_MANAGER_KEY_PROVIDER_OPENSSL_IMPL_H

#include <memory>

#include "KeyProvider.h"
#include <openssl/evp.h>

namespace auth_manager::auth::key_provider {
    class KeyProviderOpenSSLImpl : public KeyProvider {
    private:
        std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)> _private_key{ nullptr, EVP_PKEY_free };
        std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)> _public_key{ nullptr, EVP_PKEY_free };

        void extract_keys(const EVP_PKEY *pkey) const;
    public:
        KeyProviderOpenSSLImpl(const std::string& file_base, const std::string& key_name);

        void load_keys() override;
        void generate_new_keys() override;
        void free_keys() override;

        std::vector<unsigned char> sign(const std::string &message) override;
        bool verify(const std::string &message, const std::vector<unsigned char> &signature) override;

        [[nodiscard]] bool is_key_loaded() const override;

        [[nodiscard]] std::string export_public_key() const override;
    };
}

#endif //AUTH_MANAGER_KEY_PROVIDER_OPENSSL_IMPL_H
