//
// Created by yareach on 26. 6. 26..
//

#include "auth_manager/auth/factory/key_provider_factory.h"
#include <auth_manager/auth/key_provider/KeyProviderOpenSSLImpl.h>

namespace auth_manager::auth {
    using namespace auth_manager::auth::key_provider;

    KeyProvider* generate_key_provider(
        const std::string& file_base,
        const std::string& key_name
    ) {
        return new KeyProviderOpenSSLImpl(file_base, key_name);
    }
}
