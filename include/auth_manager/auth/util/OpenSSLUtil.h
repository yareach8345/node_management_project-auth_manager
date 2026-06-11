//
// Created by yareach on 26. 6. 11..
//

#ifndef AUTH_MANAGER_OPENSSL_UTIL_H
#define AUTH_MANAGER_OPENSSL_UTIL_H

#include <vector>
#include <memory>
#include <openssl/evp.h>

namespace auth_manager::auth::util {
    using EvpPkeyT = std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)>;

    class OpenSSLUtil {
    public:
        [[nodiscard]] static EvpPkeyT make_evp_pkey(EVP_PKEY* init);

        [[nodiscard]] static EvpPkeyT generate_evp_pkey();

        [[nodiscard]] static EvpPkeyT extract_public_key(const EVP_PKEY* private_key);

        static void save_evp_pkey(const EVP_PKEY* pkey, const std::string& file_path);

        static EvpPkeyT read_evp_pkey(const std::string& file_path);

        [[nodiscard]] static std::vector<std::byte> sign(const std::string &message, EVP_PKEY *private_key);

        [[nodiscard]] static bool verify(const std::string &message, const std::vector<std::byte> &signature, EVP_PKEY *public_key);

        [[nodiscard]] static std::string export_public_key(const EVP_PKEY *public_key);
    };
}

#endif //AUTH_MANAGER_OPENSSL_UTIL_H
