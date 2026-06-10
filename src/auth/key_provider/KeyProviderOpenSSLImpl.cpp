//
// Created by yareach on 26. 6. 10..
//

#include "auth_manager/auth/key_provider/KeyProviderOpenSSLImpl.h"

#include <stdexcept>
#include <memory>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <filesystem>
#include <iostream>

namespace auth_manager::auth::key_provider {
    KeyProviderOpenSSLImpl::KeyProviderOpenSSLImpl(const std::string& file_base, const std::string& key_name):
        KeyProvider(file_base, key_name)
    {
        if (pem_files_exist()) { KeyProviderOpenSSLImpl::load_keys(); }
    }

    void KeyProviderOpenSSLImpl::extract_keys(const EVP_PKEY *pkey) const {
        if (FILE *private_file = fopen(_private_key_file_path.c_str(), "wb")) {
            PEM_write_PrivateKey(private_file, pkey, nullptr, nullptr, 0, nullptr, nullptr);
            fclose(private_file);
        } else {
            perror("fopen");
        }

        if (FILE *public_file = fopen(_public_key_file_path.c_str(), "wb")) {
            PEM_write_PUBKEY(public_file, pkey);
            fclose(public_file);
        } else {
            perror("fopen");
        }
    }

    void KeyProviderOpenSSLImpl::load_keys() {
        // read Private Key
        FILE* private_key_fp = fopen(_private_key_file_path.c_str(), "r");
        if (!private_key_fp) {
            throw std::runtime_error("failed to open private key file");
        }

        const auto read_private_key = PEM_read_PrivateKey(private_key_fp, nullptr, nullptr, nullptr);
        _private_key.reset(read_private_key);
        fclose(private_key_fp);

        // read Public Key
        FILE* public_key_fp = fopen(_public_key_file_path.c_str(), "r");
        if (!public_key_fp) {
            throw std::runtime_error("failed to open public key file");
        }

        const auto read_public_key = PEM_read_PUBKEY(public_key_fp, nullptr, nullptr, nullptr);
        _public_key.reset(read_public_key);
        fclose(public_key_fp);
    }

    void KeyProviderOpenSSLImpl::generate_new_keys() {
        free_keys();
        delete_keys();

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

        std::unique_ptr<EVP_PKEY, decltype(&EVP_PKEY_free)> pkey([&ctx] {
            EVP_PKEY* pkey = nullptr;

            if (EVP_PKEY_keygen(ctx.get(), &pkey) <= 0) {
                throw std::runtime_error("EVP_PKEY_keygen");
            }

            return pkey;
        }(), EVP_PKEY_free);

        std::filesystem::create_directory(_file_base);

        extract_keys(pkey.get());

        load_keys();
    }

    void KeyProviderOpenSSLImpl::free_keys() {
        _private_key.reset();
        _public_key.reset();
    }

    std::vector<unsigned char> KeyProviderOpenSSLImpl::sign(const std::string &message) {
        if (!is_key_loaded()) { throw std::runtime_error("Key is not loaded"); }

        std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> ctx(EVP_MD_CTX_new(), EVP_MD_CTX_free);

        if (!ctx)
            throw std::runtime_error("EVP_MD_CTX_new failed");

        if (EVP_DigestSignInit(ctx.get(), nullptr, EVP_sha256(), nullptr, _private_key.get()) <= 0) {
            throw std::runtime_error("EVP_DigestSignInit failed");
        }

        if (EVP_DigestSignUpdate(ctx.get(), message.data(), message.size()) <= 0) {
            throw std::runtime_error("EVP_DigestSignUpdate failed");
        }

        size_t signature_length = 0;

        if (EVP_DigestSignFinal(ctx.get(), nullptr, &signature_length) <= 0) {
            throw std::runtime_error("EVP_DigestSignFinal failed");
        }

        std::vector<unsigned char> signature(signature_length);

        if (EVP_DigestSignFinal(ctx.get(), signature.data(), &signature_length) <= 0) {
            throw std::runtime_error("EVP_DigestSignFinal failed");
        }

        signature.resize(signature_length);

        return signature;
    }

    bool KeyProviderOpenSSLImpl::verify(const std::string &message, const std::vector<unsigned char> &signature) {
        if (!is_key_loaded()) { throw std::runtime_error("Key is not loaded"); }

        const std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> ctx(EVP_MD_CTX_new(), EVP_MD_CTX_free);

        if (!ctx) return false;

        if (EVP_DigestVerifyInit(ctx.get(), nullptr, EVP_sha256(), nullptr, _public_key.get()) <= 0) { return false; }

        if (EVP_DigestVerifyUpdate(ctx.get(), message.data(), message.size()) <= 0) { return false; }

        const int result = EVP_DigestVerifyFinal(ctx.get(), signature.data(), signature.size());

        return result == 1;
    }

    bool KeyProviderOpenSSLImpl::is_key_loaded() const {
        return _private_key && _public_key;
    }

    std::string KeyProviderOpenSSLImpl::export_public_key() const {
        if (!is_key_loaded()) { throw std::runtime_error("Key is not loaded"); }

        const std::unique_ptr<BIO, decltype(&BIO_free)> bio(BIO_new(BIO_s_mem()), BIO_free);

        PEM_write_bio_PUBKEY(bio.get(), _public_key.get());

        char* data = nullptr;
        const long len = BIO_get_mem_data(bio.get(), &data);

        std::string public_key_str(data, len);

        return public_key_str;
    }
}
