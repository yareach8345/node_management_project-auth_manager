//
// Created by yareach on 26. 5. 31..
//

#ifndef AUTH_MANAGER_I_ROOT_KEY_MANAGER_H
#define AUTH_MANAGER_I_ROOT_KEY_MANAGER_H

namespace auth_manager::auth {
    class IRootKeyManager {
    public:
        virtual ~IRootKeyManager() = default;

        virtual void generate_new_keys() = 0;

        virtual void load_keys() = 0;

        virtual void update_keys() = 0;

        virtual void delete_keys() = 0;

        virtual bool is_key_loaded() = 0;
    };
}

#endif // AUTH_MANAGER_I_ROOT_KEY_MANAGER_H
