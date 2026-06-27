//
// Created by yareach on 26. 6. 26..
//

#ifndef AUTH_MANAGER_KEY_PROVIDER_FACTORY_H
#define AUTH_MANAGER_KEY_PROVIDER_FACTORY_H

#include <auth_manager/auth/key_provider/KeyProvider.h>

namespace auth_manager::auth {
    using namespace auth_manager::auth::key_provider;

    KeyProvider* generate_key_provider(const std::string& file_base, const std::string& key_name);
}

#endif //AUTH_MANAGER_KEY_PROVIDER_FACTORY_H
