//
// Created by yareach on 26. 6. 11..
//

#include "auth_manager/auth/util/OpenSSLUtil.h"
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

namespace auth_manager::auth::util {
    EvpPkeyT OpenSSLUtil::make_evp_pkey(EVP_PKEY *init) {
        return { init, EVP_PKEY_free };
    }

    EvpPkeyT OpenSSLUtil::generate_evp_pkey() {
        const std::unique_ptr<EVP_PKEY_CTX, decltype(&EVP_PKEY_CTX_free)> ctx(EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr), EVP_PKEY_CTX_free);

        if (ctx == nullptr) {
            throw std::runtime_error("EVP_PKEY_CTX_new_id failed");
        }

        if (EVP_PKEY_keygen_init(ctx.get()) <= 0) {
            throw std::runtime_error("EVP_PKEY_keygen_init failed");
        }
        if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx.get(), 2048) <= 0) {
            throw std::runtime_error("EVP_PKEY_CTX_set_rsa_keygen_bits failed");
        }

        EVP_PKEY* pkey = nullptr;
        if (EVP_PKEY_keygen(ctx.get(), &pkey) <= 0) {
            throw std::runtime_error("EVP_PKEY_keygen");
        }

        return make_evp_pkey(pkey);
    }

    EvpPkeyT OpenSSLUtil::extract_public_key(const EVP_PKEY *private_key) {
        if (private_key == nullptr) {
            throw std::runtime_error("EVP_PKEY is nullptr");
        }

        const std::unique_ptr<BIO, decltype(&BIO_free)> bio(BIO_new(BIO_s_mem()), BIO_free);

        if (bio == nullptr) { throw std::runtime_error("BIO_new failed"); }

        if (!PEM_write_bio_PUBKEY(bio.get(), private_key)) { throw std::runtime_error("PEM_write_bio_PUBKEY failed"); }

        return make_evp_pkey(PEM_read_bio_PUBKEY(bio.get(), nullptr, nullptr, nullptr));
    }

    void OpenSSLUtil::save_evp_pkey(const EVP_PKEY *pkey, const std::string &file_path) {
        const std::unique_ptr<FILE, decltype(&fclose)> file(fopen(file_path.c_str(), "wb"), fclose);
        if (file == nullptr) {
            throw std::runtime_error("fopen failed");
        }
        PEM_write_PrivateKey(file.get(), pkey, nullptr, nullptr, 0, nullptr, nullptr);
    }

    EvpPkeyT OpenSSLUtil::read_evp_pkey(const std::string &file_path) {
        const std::unique_ptr<FILE, decltype(&fclose)> file(fopen(file_path.c_str(), "r"), fclose);

        if (file == nullptr) { throw std::runtime_error("failed to open private key file"); }

        return make_evp_pkey(PEM_read_PrivateKey(file.get(), nullptr, nullptr, nullptr));
    }

    std::vector<std::byte> OpenSSLUtil::sign(const std::string &message, EVP_PKEY *private_key) {
        const std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> ctx(EVP_MD_CTX_new(), EVP_MD_CTX_free);

        if (!ctx) {
            throw std::runtime_error("EVP_MD_CTX_new failed");
        }

        if (EVP_DigestSignInit(ctx.get(), nullptr, EVP_sha256(), nullptr, private_key) <= 0) {
            throw std::runtime_error("EVP_DigestSignInit failed");
        }

        if (EVP_DigestSignUpdate(ctx.get(), message.data(), message.size()) <= 0) {
            throw std::runtime_error("EVP_DigestSignUpdate failed");
        }

        size_t signature_length = 0;

        if (EVP_DigestSignFinal(ctx.get(), nullptr, &signature_length) <= 0) {
            throw std::runtime_error("EVP_DigestSignFinal failed");
        }

        std::vector<std::byte> signature(signature_length);

        if (EVP_DigestSignFinal(ctx.get(), reinterpret_cast<unsigned char*>(signature.data()), &signature_length) <= 0) {
            throw std::runtime_error("EVP_DigestSignFinal failed");
        }

        signature.resize(signature_length);

        return signature;
    }

    bool OpenSSLUtil::verify(const std::string &message, const std::vector<std::byte> &signature, EVP_PKEY *public_key) {
        const std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> ctx(EVP_MD_CTX_new(), EVP_MD_CTX_free);

        if (!ctx) return false;

        if (EVP_DigestVerifyInit(ctx.get(), nullptr, EVP_sha256(), nullptr, public_key) <= 0) { return false; }

        if (EVP_DigestVerifyUpdate(ctx.get(), message.data(), message.size()) <= 0) { return false; }

        const int result = EVP_DigestVerifyFinal(ctx.get(), reinterpret_cast<const unsigned char*>(signature.data()), signature.size());

        return result == 1;
    }

    std::string OpenSSLUtil::export_public_key(const EVP_PKEY *public_key) {
        const std::unique_ptr<BIO, decltype(&BIO_free)> bio(BIO_new(BIO_s_mem()), BIO_free);

        PEM_write_bio_PUBKEY(bio.get(), public_key);

        char* data = nullptr;
        const long len = BIO_get_mem_data(bio.get(), &data);

        std::string public_key_str(data, len);

        return public_key_str;
    }
}
