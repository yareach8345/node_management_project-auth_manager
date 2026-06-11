//
// Created by yareach on 26. 6. 10..
//

#include "auth_manager/auth/key_provider/KeyProvider.h"
#include <filesystem>
#include <algorithm>

namespace auth_manager::auth::key_provider {
    KeyProvider::KeyProvider(const std::string& file_base, const std::string& key_name):
        _key_name(key_name),
        _file_base(file_base + "/" + key_name),
        _private_key_file_path(file_base + "/" + key_name + "/private_key.pem"),
        _public_key_file_path(file_base + "/" + key_name + "/public_key.pem")
    {}

    KeyProvider::~KeyProvider() = default;

    void KeyProvider::remove_pem_files() const {
        if (std::filesystem::exists(_file_base)) {
            std::filesystem::remove_all(_file_base);
        }
    }

    void KeyProvider::generate_new_keys() {
        remove_keys();
        generate_keys_impl();
        std::filesystem::create_directory(_file_base);
        save_keys_impl(_private_key_file_path, _public_key_file_path);
    }

    void KeyProvider::load_keys() {
        if (!pem_files_exist()) {
            throw std::runtime_error("PEM files do not exist");
        }
        load_keys_impl(_private_key_file_path, _public_key_file_path);
    }

    void KeyProvider::remove_keys() {
        free_keys_impl();
        remove_pem_files();
    }

    std::vector<unsigned char> KeyProvider::sign(const std::string &message) {
        if (!is_key_loaded()) { throw std::runtime_error("Key not loaded"); }
        return sign_impl(message);
    }

    bool KeyProvider::verify(const std::string &message, const std::vector<unsigned char> &signature) {
        if (!is_key_loaded()) { throw std::runtime_error("Key not loaded"); }
        return verify_impl(message, signature);
    }

    bool KeyProvider::pem_files_exist() const {
        return std::ranges::all_of(
            key_file_paths(),
            [](const std::string_view file_path) { return std::filesystem::exists(file_path); }
        );
    }

    bool KeyProvider::is_key_loaded() const {
        return is_key_loaded_impl();
    }

    std::string KeyProvider::export_public_key() const {
        if (!is_key_loaded()) { throw std::runtime_error("Key not loaded"); }
        return export_public_key_impl();
    }

    std::array<std::string_view, 2> KeyProvider::key_file_paths() const { return { _public_key_file_path, _private_key_file_path }; }

    std::string_view KeyProvider::file_base() const { return _file_base; }

    std::string_view KeyProvider::key_name() const { return _key_name; }

    std::string_view KeyProvider::public_key_file_path() const { return _public_key_file_path; }

    std::string_view KeyProvider::private_key_file_path() const { return _private_key_file_path; }
}
