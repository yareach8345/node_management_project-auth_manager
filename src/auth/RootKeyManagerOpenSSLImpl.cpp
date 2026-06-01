//
// Created by yareach on 26. 5. 31..
//

#include <stdexcept>

#include "auth/RootKeyManagerOpenSSLImpl.h"

#include <iostream>
#include <ostream>
#include <filesystem>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>

namespace auth_manager::auth {
    RootKeyManagerOpenSSLImpl::RootKeyManagerOpenSSLImpl(): _is_key_loaded(false) {
        ERR_load_crypto_strings();
        OpenSSL_add_all_algorithms();

        try {
            RootKeyManagerOpenSSLImpl::load_keys();
            _is_key_loaded = true;
        } catch (std::runtime_error &e) {
            std::cerr << e.what() << std::endl;
        }
    };

    RootKeyManagerOpenSSLImpl::~RootKeyManagerOpenSSLImpl() {
        free_keys();
    }

    void RootKeyManagerOpenSSLImpl::generate_new_keys() {
        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);

        if (ctx == nullptr) {
            throw std::runtime_error("EVP_PKEY_CTX_new_id failed");
        }

        if (EVP_PKEY_keygen_init(ctx) <= 0) {
            throw std::runtime_error("EVP_PKEY_keygen_init failed");
        }
        if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0) {
            throw std::runtime_error("EVP_PKEY_CTX_set_rsa_keygen_bits failed");
        }

        EVP_PKEY* pkey = nullptr;

        if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
            throw std::runtime_error("EVP_PKEY_keygen");
        }

        extract_keys(pkey);
        load_keys();

        EVP_PKEY_CTX_free(ctx);
    }

    void RootKeyManagerOpenSSLImpl::free_keys() {
        if (_private_key != nullptr) {
            EVP_PKEY_free(_private_key);
            _private_key = nullptr;
        }
        if (_public_key != nullptr) {
            EVP_PKEY_free(_public_key);
            _public_key = nullptr;
        }
    }

    void RootKeyManagerOpenSSLImpl::extract_keys(const EVP_PKEY *pkey) {
        if (pkey == nullptr) {
            throw std::runtime_error("save keys error pkey is null");
        }

        if (FILE *private_file = fopen("root_private_key.pem", "wb")) {
            PEM_write_PrivateKey(private_file, pkey, nullptr, nullptr, 0, nullptr, nullptr);
            fclose(private_file);
        }
        if (FILE *public_file = fopen("root_public_key.pem", "wb")) {
            PEM_write_PrivateKey(public_file, pkey, nullptr, nullptr, 0, nullptr, nullptr);
            fclose(public_file);
        }
    }

    void RootKeyManagerOpenSSLImpl::load_keys() {
        free_keys();

        FILE* private_key_fp = fopen("root_private_key.pem", "r");
        if (!private_key_fp) {
            throw std::runtime_error("failed to open root private key file");
        }

        _private_key = PEM_read_PrivateKey(private_key_fp, nullptr, nullptr, nullptr);
        fclose(private_key_fp);

        FILE* public_key_fp = fopen("root_public_key.pem", "r");
        if (!public_key_fp) {
            throw std::runtime_error("failed to open root public key file");
        }

        _public_key = PEM_read_PUBKEY(public_key_fp, nullptr, nullptr, nullptr);
        fclose(public_key_fp);

        _is_key_loaded = true;
    }

    void RootKeyManagerOpenSSLImpl::update_keys() {
        generate_new_keys();
        load_keys();
    }

    void RootKeyManagerOpenSSLImpl::delete_keys() {
        free_keys();

        std::filesystem::remove("root_private_key.pem");

        std::filesystem::remove("root_public_key.pem");

        _is_key_loaded = false;
    }

    bool RootKeyManagerOpenSSLImpl::is_key_loaded() {
        return _is_key_loaded;
    }
}
