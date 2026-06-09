//
// Created by yareach on 26. 6. 7..
//

#include "auth_manager/auth/service/KeyServiceOpenSSLImpl.h"

#include <chrono>
#include <format>
#include <iostream>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <filesystem>

#include "auth_manager/auth/config/AuthConfig.h"
#include "auth_manager/auth/mapping/KeysInfoJsonConverter.h"

namespace auth_manager::auth {
    void KeyServiceOpenSSLImpl::EVP_PKEY_Deleter::operator()(EVP_PKEY *p) const {
        EVP_PKEY_free(p);
    }

    KeyServiceOpenSSLImpl::KeyServiceOpenSSLImpl(const AuthConfig &auth_config, std::string key_name):
        _key_name(std::move(key_name)),
        _private_key_file_path(auth_config.file_base() + "/" + key_name + "/private_key.pem"),
        _public_key_file_path(auth_config.file_base() + "/" + key_name + "/public_key.pem"),
        _keys_info_file_path(auth_config.file_base() + "/" + key_name + "/keys_info.json"),
        _keys_info(std::nullopt),
        _json_file_manager(new core::json::JsonFileManager(KeysInfoJsonConverter::get_instance()))
    {
        const bool is_all_required_files_exist = std::ranges::all_of(
            required_files(),
            [](const std::string_view file_path) { return std::filesystem::exists(file_path); }
        );

        if (is_all_required_files_exist) {
            KeyServiceOpenSSLImpl::load_keys();
        }
    };

    KeyServiceOpenSSLImpl::~KeyServiceOpenSSLImpl() = default;

    void KeyServiceOpenSSLImpl::generate_new_keys() {
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
        _json_file_manager->write_to_file(_keys_info_file_path, KeysInfo(created_at));

        load_keys();

        EVP_PKEY_CTX_free(ctx);
    }

    void KeyServiceOpenSSLImpl::clear() {
        _private_key.reset();
        _public_key.reset();
        _keys_info.reset();
    }

    void KeyServiceOpenSSLImpl::extract_keys(const EVP_PKEY *pkey) const {
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
            PEM_write_PUBKEY(public_file, pkey);
            fclose(public_file);
        } else {
            perror("fopen");
        }
    }

    std::array<std::string_view, 3> KeyServiceOpenSSLImpl::required_files() const {
        return { _private_key_file_path, _public_key_file_path, _keys_info_file_path };
    }

    void KeyServiceOpenSSLImpl::load_keys() {
        clear();

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

        //read keys info
        _keys_info = _json_file_manager->read_from_file(_keys_info_file_path);
    }

    void KeyServiceOpenSSLImpl::update_keys() {
        generate_new_keys();
        load_keys();
    }

    void KeyServiceOpenSSLImpl::delete_keys() {
        clear();

        for (std::string_view required_file : required_files()) {
            if (std::filesystem::exists(required_file)) {
                std::filesystem::remove(required_file);
            }
        }
    }

    std::vector<unsigned char> KeyServiceOpenSSLImpl::sign(const std::string &message) {
        if (!is_key_loaded()) {
            throw std::runtime_error("Key is not loaded");
        }

        EVP_MD_CTX* ctx = EVP_MD_CTX_new();

        if (!ctx)
            throw std::runtime_error("EVP_MD_CTX_new failed");

        if (EVP_DigestSignInit(ctx, nullptr, EVP_sha256(), nullptr, _private_key.get()) <= 0) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("EVP_DigestSignInit failed");
        }

        if (EVP_DigestSignUpdate(ctx, message.data(), message.size()) <= 0) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("EVP_DigestSignUpdate failed");
        }

        size_t signature_length = 0;

        if (EVP_DigestSignFinal(ctx, nullptr, &signature_length) <= 0) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("EVP_DigestSignFinal failed");
        }

        std::vector<unsigned char> signature(signature_length);

        if (EVP_DigestSignFinal(ctx, signature.data(), &signature_length) <= 0) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("EVP_DigestSignFinal failed");
        }

        signature.resize(signature_length);

        EVP_MD_CTX_free(ctx);

        return signature;
    }

    bool KeyServiceOpenSSLImpl::verify(const std::string &message, const std::vector<unsigned char> &signature) {
        if (!is_key_loaded()) {
            throw std::runtime_error("Key is not loaded");
        }

        EVP_MD_CTX* ctx = EVP_MD_CTX_new();

        if (!ctx) return false;

        if (EVP_DigestVerifyInit(ctx, nullptr, EVP_sha256(), nullptr, _public_key.get()) <= 0) {
            EVP_MD_CTX_free(ctx);
            return false;
        }

        if (EVP_DigestVerifyUpdate(ctx, message.data(), message.size()) <= 0) {
            EVP_MD_CTX_free(ctx);
            return false;
        }

        const int result = EVP_DigestVerifyFinal(ctx, signature.data(), signature.size());

        EVP_MD_CTX_free(ctx);

        return result == 1;
    }

    bool KeyServiceOpenSSLImpl::is_key_loaded() const { return _private_key && _public_key && _keys_info; }

    std::string_view KeyServiceOpenSSLImpl::key_name() const { return _key_name; }

    std::string_view KeyServiceOpenSSLImpl::private_key_file_path() const { return _private_key_file_path; }

    std::string_view KeyServiceOpenSSLImpl::public_key_file_path() const { return _public_key_file_path; }

    std::string KeyServiceOpenSSLImpl::export_public_key() const {
        if (!is_key_loaded()) {
            throw std::runtime_error("Key is not loaded");
        }

        BIO* bio = BIO_new(BIO_s_mem());

        PEM_write_bio_PUBKEY(bio, _public_key.get());

        char* data = nullptr;
        const long len = BIO_get_mem_data(bio, &data);

        std::string public_key_str(data, len);

        BIO_free(bio);

        return public_key_str;
    }

    std::string_view KeyServiceOpenSSLImpl::keys_info_file_path() const { return _keys_info_file_path; }

    std::optional<KeysInfo> KeyServiceOpenSSLImpl::keys_info() const { return _keys_info; }
}
