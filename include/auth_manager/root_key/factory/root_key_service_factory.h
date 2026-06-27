//
// Created by yareach on 26. 6. 26..
//

#ifndef AUTH_MANAGER_ROOT_KEY_SERVICE_FACTORY_H
#define AUTH_MANAGER_ROOT_KEY_SERVICE_FACTORY_H

#include "auth_manager/auth/config/AuthConfig.h"
#include "auth_manager/auth/service/IKeyService.h"

namespace auth_manager::root_key {
    using namespace auth_manager::auth;

    IKeyService* generate_root_key_service(const AuthConfig& auth_config);
}

#endif //AUTH_MANAGER_ROOT_KEY_SERVICE_FACTORY_H
