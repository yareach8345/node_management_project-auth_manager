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

    protected:
        void free_keys_impl() override;

        void generate_keys_impl() override;
        void save_keys_impl(const std::string& private_key_path, const std::string& public_key_path) override;
        void load_keys_impl(const std::string& private_key_path, const std::string& public_key_path) override;

        std::vector<unsigned char> sign_impl(const std::string &message) override;
        bool verify_impl(const std::string &message, const std::vector<unsigned char> &signature) override;

        [[nodiscard]] bool is_key_loaded_impl() const override;
        [[nodiscard]] std::string export_public_key_impl() const override;

    public:
        KeyProviderOpenSSLImpl(const std::string& file_base, const std::string& key_name);
    };
}

#endif //AUTH_MANAGER_KEY_PROVIDER_OPENSSL_IMPL_H
