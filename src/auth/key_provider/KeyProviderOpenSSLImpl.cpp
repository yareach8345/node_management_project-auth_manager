//
// Created by yareach on 26. 6. 10..
//

#include "auth_manager/auth/key_provider/KeyProviderOpenSSLImpl.h"
#include "auth_manager/auth/util/OpenSSLUtil.h"

using namespace auth_manager::auth::util;

namespace auth_manager::auth::key_provider {
    KeyProviderOpenSSLImpl::KeyProviderOpenSSLImpl(const std::string& file_base, const std::string& key_name):
        KeyProvider(file_base, key_name)
    {
        if (pem_files_exist()) { KeyProviderOpenSSLImpl::load_keys(); }
    }


    void KeyProviderOpenSSLImpl::free_keys_impl() {
        _private_key.reset();
        _public_key.reset();
    }

    void KeyProviderOpenSSLImpl::generate_keys_impl() {
        _private_key = OpenSSLUtil::generate_evp_pkey();
        _public_key = OpenSSLUtil::extract_public_key(_private_key.get());
    }

    void KeyProviderOpenSSLImpl::save_keys_impl(const std::string& private_key_path, const std::string& public_key_path) {
        OpenSSLUtil::save_evp_pkey(_private_key.get(), private_key_path);
        OpenSSLUtil::save_evp_pkey(_public_key.get(), public_key_path);
    }

    void KeyProviderOpenSSLImpl::load_keys_impl(const std::string& private_key_path, const std::string& public_key_path) {
        _private_key = OpenSSLUtil::read_evp_pkey(private_key_path);
        _public_key = OpenSSLUtil::read_evp_pkey(public_key_path);
    }

    std::vector<std::byte> KeyProviderOpenSSLImpl::sign_impl(const std::string &message) {
        return OpenSSLUtil::sign(message, _private_key.get());
    }

    bool KeyProviderOpenSSLImpl::verify_impl(const std::string &message, const std::vector<std::byte> &signature) {
        return OpenSSLUtil::verify(message, signature, _public_key.get());
    }

    bool KeyProviderOpenSSLImpl::is_key_loaded_impl() const {
        return _private_key && _public_key;
    }

    std::string KeyProviderOpenSSLImpl::export_public_key_impl() const {
        return OpenSSLUtil::export_public_key(_public_key.get());
    }
}
