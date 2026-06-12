//
// Created by yareach on 26. 6. 10..
//

#ifndef AUTH_MANAGER_AUTH_KEY_MANAGER_H
#define AUTH_MANAGER_AUTH_KEY_MANAGER_H

#include <string>
#include <array>
#include <vector>
#include <cstddef>
#include <memory>
#include "auth_manager/auth/key_provider/KeyProvider.h"

namespace auth_manager::auth {
    class AuthKeyManager {
    private:
        void remove_pem_files() const;
    protected:
        const std::shared_ptr<key_provider::KeyProvider> _key_provider;

        const std::string _key_name;
        const std::string _file_base;
        const std::string _private_key_file_path;
        const std::string _public_key_file_path;

        [[nodiscard]] std::array<std::string_view, 2> key_file_paths() const;
    public:
        explicit AuthKeyManager(
            const std::shared_ptr<key_provider::KeyProvider>& key_provider,
            const std::string& file_base,
            const std::string& key_name
        );

        void generate_new_keys();
        void load_keys();
        void remove_keys();

        std::vector<std::byte> sign(const std::string &message);
        bool verify(const std::string& message, const std::vector<std::byte>& signature);

        [[nodiscard]] bool is_key_loaded() const;

        [[nodiscard]] bool pem_files_exist() const;

        [[nodiscard]] std::string_view file_base() const;
        [[nodiscard]] std::string_view key_name() const;

        [[nodiscard]] std::string_view private_key_file_path() const;
        [[nodiscard]] std::string_view public_key_file_path() const;

        [[nodiscard]] std::string export_public_key() const;
    };
}

#endif //AUTH_MANAGER_AUTH_KEY_MANAGER_H
