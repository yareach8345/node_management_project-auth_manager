//
// Created by yareach on 26. 6. 7..
//

#include "auth_manager/auth/service/KeyServiceImpl.h"

#include <chrono>
#include <format>
#include <filesystem>
#include <utility>

#include "auth_manager/auth/mapping/KeysInfoJsonConverter.h"
#include "auth_manager/auth/util/OpenSSLUtil.h"

namespace auth_manager::auth {
    KeyServiceImpl::KeyServiceImpl(
        std::unique_ptr<key_provider::KeyProvider> key_provider,
        core::json::JsonFileManager<KeysInfo> _json_file_manager
    ):
        _key_provider(std::move(key_provider)),
        _json_file_manager(std::move(_json_file_manager))
    {
        if (_key_provider->is_key_loaded() && _json_file_manager.exists()) {
            KeyServiceImpl::load_keys();
        }
    };

    KeyServiceImpl::~KeyServiceImpl() = default;

    void KeyServiceImpl::generate_new_keys() {
        _key_provider->generate_new_keys();

        const auto now = std::chrono::system_clock::now();
        const std::string created_at = std::format("{:%Y-%m-%d %H:%M:%S}", now);
        _json_file_manager.write_to_file(KeysInfo(created_at, util::OpenSSLUtil::ALGORITHM, util::OpenSSLUtil::KEY_BITS));

        load_keys();
    }

    std::array<std::filesystem::path, 3> KeyServiceImpl::required_files() const {
        return { _key_provider->private_key_file_path(), _key_provider->public_key_file_path(),  _json_file_manager.file_path() };
    }

    void KeyServiceImpl::load_keys() {
        _key_provider->load_keys();
        _keys_info = _json_file_manager.read_from_file();
    }

    void KeyServiceImpl::update_keys() {
        generate_new_keys();
        load_keys();
    }

    void KeyServiceImpl::delete_keys() {
        _key_provider->remove_keys();
        _json_file_manager.delete_file();
        _keys_info.reset();
    }

    std::vector<std::byte> KeyServiceImpl::sign(const std::string &message) {
        return _key_provider->sign(message);
    }

    bool KeyServiceImpl::verify(const std::string &message, const std::vector<std::byte> &signature) {
        return _key_provider->verify(message, signature);
    }

    bool KeyServiceImpl::is_key_loaded() const { return _key_provider->is_key_loaded() && _keys_info; }

    std::string_view KeyServiceImpl::key_name() const { return _key_provider->key_name(); }

    std::filesystem::path KeyServiceImpl::private_key_file_path() const { return _key_provider->private_key_file_path(); }

    std::filesystem::path KeyServiceImpl::public_key_file_path() const { return _key_provider->public_key_file_path(); }

    std::optional<std::string> KeyServiceImpl::export_public_key() const {
        if (!_key_provider->is_key_loaded()) {
            return std::nullopt;
        }
        return _key_provider->export_public_key();
    }

    std::filesystem::path KeyServiceImpl::keys_info_file_path() const { return _json_file_manager.file_path(); }

    std::optional<KeysInfo> KeyServiceImpl::keys_info() const { return _keys_info; }
}
