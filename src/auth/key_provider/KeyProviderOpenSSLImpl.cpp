//
// Created by yareach on 26. 6. 10..
//

#include "auth_manager/auth/key_provider/KeyProviderOpenSSLImpl.h"
#include "auth_manager/auth/util/OpenSSLUtil.h"

using namespace auth_manager::auth::util;

namespace auth_manager::auth::key_provider {
    void KeyProviderOpenSSLImpl::free_keys() {
        _private_key.reset();
        _public_key.reset();
    }

    void KeyProviderOpenSSLImpl::generate_keys() {
        _private_key = OpenSSLUtil::generate_evp_pkey();
        _public_key = OpenSSLUtil::extract_public_key(_private_key.get());
    }

    void KeyProviderOpenSSLImpl::save_keys(const std::string& private_key_path, const std::string& public_key_path) {
        OpenSSLUtil::save_evp_pkey(_private_key.get(), private_key_path);
        OpenSSLUtil::save_evp_pkey(_public_key.get(), public_key_path);
    }

    void KeyProviderOpenSSLImpl::load_keys(const std::string& private_key_path, const std::string& public_key_path) {
        _private_key = OpenSSLUtil::read_evp_pkey(private_key_path);
        _public_key = OpenSSLUtil::read_evp_pkey(public_key_path);
    }

    std::vector<std::byte> KeyProviderOpenSSLImpl::sign(const std::string &message) {
        return OpenSSLUtil::sign(message, _private_key.get());
    }

    bool KeyProviderOpenSSLImpl::verify(const std::string &message, const std::vector<std::byte> &signature) {
        return OpenSSLUtil::verify(message, signature, _public_key.get());
    }

    bool KeyProviderOpenSSLImpl::is_key_loaded() const {
        return _private_key && _public_key;
    }

    std::string KeyProviderOpenSSLImpl::export_public_key() const {
        return OpenSSLUtil::export_public_key(_public_key.get());
    }
}
