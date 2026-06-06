//
// Created by yareach on 26. 6. 7..
//

#ifndef AUTH_MANAGER_KEY_SERVICE_OPENSSL_IMPL_H
#define AUTH_MANAGER_KEY_SERVICE_OPENSSL_IMPL_H

#include "IKeyService.h"
#include <openssl/evp.h>

#include "auth_manager/auth/config/AuthConfig.h"

namespace auth_manager::auth {
    class KeyServiceOpenSSLImpl : public IKeyService {
    private:
        struct EVP_PKEY_Deleter {
            void operator()(EVP_PKEY* p) const;
        };
        using EVPKeyPointer = std::unique_ptr<EVP_PKEY, EVP_PKEY_Deleter>;

        const std::string _private_key_file_path;
        const std::string _public_key_file_path;
        const std::string _keys_info_file_path;

        const std::string _key_name;

        EVPKeyPointer _private_key = nullptr;
        EVPKeyPointer _public_key = nullptr;

        std::optional<RootKeysInfo> _root_keys_info;

        void extract_keys(const EVP_PKEY *pkey) const;
        void clear();
    public:
        explicit KeyServiceOpenSSLImpl(const AuthConfig &auth_config, std::string key_name);

        ~KeyServiceOpenSSLImpl() override;

        void generate_new_keys() override;
        void load_keys() override;
        void update_keys() override;
        void delete_keys() override;

        std::vector<unsigned char> sign(const std::string &message) override;

        bool verify(const std::string& message, const std::vector<unsigned char>& signature) override;

        [[nodiscard]] std::string_view key_name() const override;

        [[nodiscard]] bool is_key_loaded() const override;

        [[nodiscard]] std::string_view private_key_file_path() const override;
        [[nodiscard]] std::string_view public_key_file_path() const override;
        [[nodiscard]] std::string export_public_key() const override;

        [[nodiscard]] std::string_view keys_info_file_path() const override;
        [[nodiscard]] std::optional<RootKeysInfo> root_keys_info() const override;
    };
}

#endif //AUTH_MANAGER_KEY_SERVICE_OPENSSL_IMPL_H
