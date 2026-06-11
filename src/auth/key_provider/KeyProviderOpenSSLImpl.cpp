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


    void KeyProviderOpenSSLImpl::free_keys_impl() {
        _private_key.reset();
        _public_key.reset();
    }

    void KeyProviderOpenSSLImpl::generate_keys_impl() {
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
        _private_key.reset(pkey);

        const std::unique_ptr<BIO, decltype(&BIO_free)> bio(BIO_new(BIO_s_mem()), BIO_free);

        if (bio == nullptr) { throw std::runtime_error("BIO_new failed"); }

        if (!PEM_write_bio_PUBKEY(bio.get(), pkey)) { throw std::runtime_error("PEM_write_bio_PUBKEY failed"); }

        _public_key.reset(PEM_read_bio_PUBKEY(bio.get(), nullptr, nullptr, nullptr));
        if (_public_key == nullptr) {
            remove_keys();
            throw std::runtime_error("PEM_read_bio_PUBKEY failed");
        }
    }

    void KeyProviderOpenSSLImpl::save_keys_impl(const std::string& private_key_path, const std::string& public_key_path) {
        if (FILE *private_file = fopen(private_key_path.c_str(), "wb")) {
            PEM_write_PrivateKey(private_file, _private_key.get(), nullptr, nullptr, 0, nullptr, nullptr);
            fclose(private_file);
        } else {
            perror("fopen");
        }
        if (FILE *public_file = fopen(public_key_path.c_str(), "wb")) {
            PEM_write_PUBKEY(public_file, _public_key.get());
            fclose(public_file);
        } else {
            perror("fopen");
        }
    }

    void KeyProviderOpenSSLImpl::load_keys_impl(const std::string& private_key_path, const std::string& public_key_path) {
        // read Private Key
        FILE* private_key_fp = fopen(private_key_path.c_str(), "r");
        if (!private_key_fp) {
            throw std::runtime_error("failed to open private key file");
        }

        const auto read_private_key = PEM_read_PrivateKey(private_key_fp, nullptr, nullptr, nullptr);
        _private_key.reset(read_private_key);
        fclose(private_key_fp);

        // read Public Key
        FILE* public_key_fp = fopen(public_key_path.c_str(), "r");
        if (!public_key_fp) {
            throw std::runtime_error("failed to open public key file");
        }

        const auto read_public_key = PEM_read_PUBKEY(public_key_fp, nullptr, nullptr, nullptr);
        _public_key.reset(read_public_key);
        fclose(public_key_fp);
    }

    std::vector<std::byte> KeyProviderOpenSSLImpl::sign_impl(const std::string &message) {
        const std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> ctx(EVP_MD_CTX_new(), EVP_MD_CTX_free);

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

        std::vector<std::byte> signature(signature_length);

        if (EVP_DigestSignFinal(ctx.get(), reinterpret_cast<unsigned char*>(signature.data()), &signature_length) <= 0) {
            throw std::runtime_error("EVP_DigestSignFinal failed");
        }

        signature.resize(signature_length);

        return signature;
    }

    bool KeyProviderOpenSSLImpl::verify_impl(const std::string &message, const std::vector<std::byte> &signature) {
        const std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)> ctx(EVP_MD_CTX_new(), EVP_MD_CTX_free);

        if (!ctx) return false;

        if (EVP_DigestVerifyInit(ctx.get(), nullptr, EVP_sha256(), nullptr, _public_key.get()) <= 0) { return false; }

        if (EVP_DigestVerifyUpdate(ctx.get(), message.data(), message.size()) <= 0) { return false; }

        const int result = EVP_DigestVerifyFinal(ctx.get(), reinterpret_cast<const unsigned char*>(signature.data()), signature.size());

        return result == 1;
    }

    bool KeyProviderOpenSSLImpl::is_key_loaded_impl() const {
        return _private_key && _public_key;
    }

    std::string KeyProviderOpenSSLImpl::export_public_key_impl() const {
        const std::unique_ptr<BIO, decltype(&BIO_free)> bio(BIO_new(BIO_s_mem()), BIO_free);

        PEM_write_bio_PUBKEY(bio.get(), _public_key.get());

        char* data = nullptr;
        const long len = BIO_get_mem_data(bio.get(), &data);

        std::string public_key_str(data, len);

        return public_key_str;
    }
}
