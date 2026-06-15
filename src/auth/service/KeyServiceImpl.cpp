//
// Created by yareach on 26. 6. 7..
//

#include "auth_manager/auth/service/KeyServiceImpl.h"

#include <chrono>
#include <format>
#include <iostream>
#include <filesystem>
#include <utility>

#include "auth_manager/auth/config/AuthConfig.h"
#include "auth_manager/auth/mapping/KeysInfoJsonConverter.h"

namespace auth_manager::auth {
    KeyServiceImpl::KeyServiceImpl(
        std::shared_ptr<key_provider::KeyProvider> key_provider,
        const AuthConfig &auth_config,
        const std::string& key_name
    ):
        _key_provider(std::move(key_provider)),
        _keys_info_file_path(auth_config.file_base() + "/" + key_name + "/keys_info.json"),
        _json_file_manager(auth_config.file_base() + "/" + key_name + "/keys_info.json", KeysInfoJsonConverter::get_instance())
    {
        const bool is_all_required_files_exist = std::ranges::all_of(
            required_files(),
            [](const std::string_view file_path) { return std::filesystem::exists(file_path); }
        );

        if (is_all_required_files_exist) {
            KeyServiceImpl::load_keys();
        }
    };

    KeyServiceImpl::~KeyServiceImpl() = default;

    void KeyServiceImpl::generate_new_keys() {
        _key_provider->generate_new_keys();

        const auto now = std::chrono::system_clock::now();
        const std::string created_at = std::format("{:%Y-%m-%d %H:%M:%S}", now);
        _json_file_manager.write_to_file(KeysInfo(created_at));

        load_keys();
    }

    std::array<std::string_view, 3> KeyServiceImpl::required_files() const {
        return { _key_provider->private_key_file_path().string(), _key_provider->public_key_file_path().string(), _keys_info_file_path };
    }

    void KeyServiceImpl::load_keys() {
        _key_provider->load_keys();

        //read keys info
        _keys_info = _json_file_manager.read_from_file();
    }

    void KeyServiceImpl::update_keys() {
        generate_new_keys();
        load_keys();
    }

    void KeyServiceImpl::delete_keys() {
        for (std::string_view required_file : required_files()) {
            if (std::filesystem::exists(required_file)) {
                std::filesystem::remove(required_file);
            }
        }
    }

    std::vector<std::byte> KeyServiceImpl::sign(const std::string &message) {
        return _key_provider->sign(message);
    }

    bool KeyServiceImpl::verify(const std::string &message, const std::vector<std::byte> &signature) {
        return _key_provider->verify(message, signature);
    }

    bool KeyServiceImpl::is_key_loaded() const { return _key_provider->is_key_loaded() && _keys_info; }

    std::string_view KeyServiceImpl::key_name() const { return _key_provider->key_name(); }

    std::string_view KeyServiceImpl::private_key_file_path() const { return _key_provider->private_key_file_path().c_str(); }

    std::string_view KeyServiceImpl::public_key_file_path() const { return _key_provider->public_key_file_path().c_str(); }

    std::string KeyServiceImpl::export_public_key() const { return _key_provider->export_public_key(); }

    std::string_view KeyServiceImpl::keys_info_file_path() const { return _keys_info_file_path; }

    std::optional<KeysInfo> KeyServiceImpl::keys_info() const { return _keys_info; }
}
