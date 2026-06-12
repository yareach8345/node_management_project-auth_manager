//
// Created by yareach on 26. 6. 12..
//

#include "../../../include/auth_manager/auth/key_manager/AuthKeyManager.h"

#include <algorithm>
#include <filesystem>

namespace auth_manager::auth {
    AuthKeyManager::AuthKeyManager(const std::shared_ptr<key_provider::KeyProvider> &key_provider, const std::string &file_base, const std::string &key_name) :
        _key_provider(key_provider),
        _key_name(key_name),
        _file_base(file_base + "/" + key_name),
        _private_key_file_path(file_base + "/" + key_name + "/private_key.pem"),
        _public_key_file_path(file_base + "/" + key_name + "/public_key.pem")
    {}

    void AuthKeyManager::remove_pem_files() const {
        if (std::filesystem::exists(_file_base)) {
            std::filesystem::remove_all(_file_base);
        }
    }

    void AuthKeyManager::generate_new_keys() {
        remove_keys();
        _key_provider->generate_keys();
        std::filesystem::create_directory(_file_base);
        _key_provider->save_keys(_private_key_file_path, _public_key_file_path);
    }

    void AuthKeyManager::load_keys() {
        if (!pem_files_exist()) {
            throw std::runtime_error("PEM files do not exist");
        }
        _key_provider->load_keys(_private_key_file_path, _public_key_file_path);
    }

    void AuthKeyManager::remove_keys() {
        _key_provider->free_keys();
        remove_pem_files();
    }

    std::vector<std::byte> AuthKeyManager::sign(const std::string &message) {
        if (!is_key_loaded()) { throw std::runtime_error("Key not loaded"); }
        return _key_provider->sign(message);
    }

    bool AuthKeyManager::verify(const std::string &message, const std::vector<std::byte> &signature) {
        if (!is_key_loaded()) { throw std::runtime_error("Key not loaded"); }
        return _key_provider->verify(message, signature);
    }

    bool AuthKeyManager::pem_files_exist() const {
        return std::ranges::all_of(
            key_file_paths(),
            [](const std::string_view file_path) { return std::filesystem::exists(file_path); }
        );
    }

    bool AuthKeyManager::is_key_loaded() const {
        return _key_provider->is_key_loaded();
    }

    std::string AuthKeyManager::export_public_key() const {
        if (!is_key_loaded()) { throw std::runtime_error("Key not loaded"); }
        return _key_provider->export_public_key();
    }

    std::array<std::string_view, 2> AuthKeyManager::key_file_paths() const { return { _public_key_file_path, _private_key_file_path }; }

    std::string_view AuthKeyManager::file_base() const { return _file_base; }

    std::string_view AuthKeyManager::key_name() const { return _key_name; }

    std::string_view AuthKeyManager::public_key_file_path() const { return _public_key_file_path; }

    std::string_view AuthKeyManager::private_key_file_path() const { return _private_key_file_path; }
}