//
// Created by yareach on 26. 5. 31..
//

#ifndef AUTH_MANAGER_I_ROOT_KEY_MANAGER_H
#define AUTH_MANAGER_I_ROOT_KEY_MANAGER_H
#include "RootKeysInfo.h"

namespace auth_manager::auth {
    template<typename PrivateKeyT, typename PublicKeyT>
    class IRootKeyManager {
    public:
        virtual ~IRootKeyManager() = default;

        virtual void generate_new_keys() = 0;
        virtual void load_keys() = 0;
        virtual void update_keys() = 0;
        virtual void delete_keys() = 0;

        [[nodiscard]] virtual bool is_key_loaded() const = 0;

        [[nodiscard]] virtual std::string_view private_key_file_path() const = 0;
        [[nodiscard]] virtual PrivateKeyT private_key() const = 0;

        [[nodiscard]] virtual std::string_view public_key_file_path() const = 0;
        [[nodiscard]] virtual PublicKeyT public_key() const = 0;

        [[nodiscard]] virtual std::string_view keys_info_file_path() const = 0;
        [[nodiscard]] virtual const std::optional<RootKeysInfo>& root_keys_info() const = 0;
    };
}

#endif // AUTH_MANAGER_I_ROOT_KEY_MANAGER_H
