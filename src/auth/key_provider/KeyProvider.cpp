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

    void KeyProvider::delete_keys() {
        free_keys();
        std::filesystem::remove_all(_file_base);
    }

    bool KeyProvider::pem_files_exist() const {
        return std::ranges::all_of(
            key_file_paths(),
            [](const std::string_view file_path) { return std::filesystem::exists(file_path); }
        );
    }

    std::array<std::string_view, 2> KeyProvider::key_file_paths() const { return { _public_key_file_path, _private_key_file_path }; }

    std::string_view KeyProvider::file_base() const { return _file_base; }

    std::string_view KeyProvider::key_name() const { return _key_name; }

    std::string_view KeyProvider::public_key_file_path() const { return _public_key_file_path; }

    std::string_view KeyProvider::private_key_file_path() const { return _private_key_file_path; }
}
