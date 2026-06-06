//
// Created by yareach on 26. 6. 7..
//

#ifndef AUTH_MANAGER_I_KEY_SERVICE_H
#define AUTH_MANAGER_I_KEY_SERVICE_H
#include <optional>
#include <string_view>

#include "auth_manager/auth/data/RootKeysInfo.h"

namespace auth_manager::auth {
    class IKeyService {
    public:
        virtual ~IKeyService() = default;

        virtual void generate_new_keys() = 0;

        virtual void load_keys() = 0;

        virtual void update_keys() = 0;

        virtual void delete_keys() = 0;

        virtual std::vector<unsigned char> sign(const std::string& message) = 0;

        virtual bool verify(const std::string& message, const std::vector<unsigned char>& signature) = 0;

        [[nodiscard]] virtual std::string_view key_name() const = 0;

        [[nodiscard]] virtual bool is_key_loaded() const = 0;

        [[nodiscard]] virtual std::string_view private_key_file_path() const = 0;
        [[nodiscard]] virtual std::string_view public_key_file_path() const = 0;
        [[nodiscard]] virtual std::string export_public_key() const = 0;

        [[nodiscard]] virtual std::string_view keys_info_file_path() const = 0;
        [[nodiscard]] virtual std::optional<RootKeysInfo> root_keys_info() const = 0;
    };
}

#endif //AUTH_MANAGER_I_KEY_SERVICE_H
