//
// Created by yareach on 26. 6. 10..
//

#ifndef AUTH_MANAGER_KEY_PROVIDER_H
#define AUTH_MANAGER_KEY_PROVIDER_H

#include <string>
#include <vector>
#include <cstddef>

namespace auth_manager::auth::key_provider {
    class KeyProvider {
    public:
        virtual ~KeyProvider() = default;

        virtual void free_keys() = 0;

        virtual void generate_keys() = 0;
        virtual void save_keys(const std::string& private_key_path, const std::string& public_key_path) = 0;
        virtual void load_keys(const std::string& private_key_path, const std::string& public_key_path) = 0;

        virtual std::vector<std::byte> sign(const std::string &message) = 0;
        virtual bool verify(const std::string &message, const std::vector<std::byte> &signature) = 0;

        [[nodiscard]] virtual bool is_key_loaded() const = 0;

        [[nodiscard]] virtual std::string export_public_key() const = 0;
    };
}

#endif //AUTH_MANAGER_KEY_PROVIDER_H
