//
// Created by yareach on 26. 5. 31..
//

#include <stdexcept>

#include "auth/root_key_manager/RootKeyManagerOpenSSLImpl.h"

#include <iostream>
#include <filesystem>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <chrono>
#include <format>
#include <nlohmann/json.hpp>
#include <algorithm>

#include "auth/root_key_manager/RootKeysInfo.h"
#include "util/JsonUtil.h"

namespace auth_manager::auth {
    RootKeyManagerOpenSSLImpl::RootKeyManagerOpenSSLImpl(const AuthConfig &auth_config):
        _private_key_file_path((auth_config.file_base() + "/root_private_key.pem")),
        _public_key_file_path((auth_config.file_base() + "/root_public_key.pem")),
        _keys_info_file_path((auth_config.file_base() + "/root_keys_info.json")),
        _is_key_loaded(false),
        _root_keys_info(std::nullopt)
    {
        ERR_load_crypto_strings();
        OpenSSL_add_all_algorithms();

        const std::string_view required_files[] = {
            _private_key_file_path,
            _public_key_file_path,
            _keys_info_file_path,
        };

        const bool is_all_required_files_exist = std::ranges::all_of(
            required_files,
            [](const std::string_view file_path) { return std::filesystem::exists(file_path); }
        );

        if (is_all_required_files_exist) {
            RootKeyManagerOpenSSLImpl::load_keys();
            _is_key_loaded = true;
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

        const auto now = std::chrono::system_clock::now();
        const std::string created_at = std::format("{:%Y-%m-%d %H:%M:%S}", now);
        nlohmann::json keys_info_json = RootKeysInfo(created_at).to_json();
        util::JsonUtil::save_json_file(_keys_info_file_path, keys_info_json);

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

    void RootKeyManagerOpenSSLImpl::extract_keys(const EVP_PKEY *pkey) const {
        if (pkey == nullptr) {
            throw std::runtime_error("save keys error pkey is null");
        }

        if (FILE *private_file = fopen(_private_key_file_path.c_str(), "wb")) {
            PEM_write_PrivateKey(private_file, pkey, nullptr, nullptr, 0, nullptr, nullptr);
            fclose(private_file);
        } else {
            perror("fopen");
        }

        if (FILE *public_file = fopen(_public_key_file_path.c_str(), "wb")) {
            PEM_write_PrivateKey(public_file, pkey, nullptr, nullptr, 0, nullptr, nullptr);
            fclose(public_file);
        } else {
            perror("fopen");
        }
    }

    void RootKeyManagerOpenSSLImpl::load_keys() {
        free_keys();

        FILE* private_key_fp = fopen(_private_key_file_path.c_str(), "r");
        if (!private_key_fp) {
            throw std::runtime_error("failed to open root private key file");
        }

        _private_key = PEM_read_PrivateKey(private_key_fp, nullptr, nullptr, nullptr);
        fclose(private_key_fp);

        FILE* public_key_fp = fopen(_public_key_file_path.c_str(), "r");
        if (!public_key_fp) {
            throw std::runtime_error("failed to open root public key file");
        }

        _public_key = PEM_read_PUBKEY(public_key_fp, nullptr, nullptr, nullptr);
        fclose(public_key_fp);

        _root_keys_info = RootKeysInfo::from_json(util::JsonUtil::load_json_file(_keys_info_file_path));

        _is_key_loaded = true;
    }

    void RootKeyManagerOpenSSLImpl::update_keys() {
        generate_new_keys();
        load_keys();
    }

    void RootKeyManagerOpenSSLImpl::delete_keys() {
        free_keys();

        std::filesystem::remove(_private_key_file_path);

        std::filesystem::remove(_public_key_file_path);

        _is_key_loaded = false;
    }

    bool RootKeyManagerOpenSSLImpl::is_key_loaded() {
        return _is_key_loaded;
    }

    const std::string& RootKeyManagerOpenSSLImpl::private_key_file_path() {
        return _private_key_file_path;
    }

    const std::string& RootKeyManagerOpenSSLImpl::public_key_file_path() {
        return _public_key_file_path;
    }

    const std::string& RootKeyManagerOpenSSLImpl::keys_info_file_path() {
        return _keys_info_file_path;
    }

    const std::optional<RootKeysInfo>& RootKeyManagerOpenSSLImpl::root_keys_info() {
        return _root_keys_info;
    }
}
