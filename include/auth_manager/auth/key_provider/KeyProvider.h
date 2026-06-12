//
// Created by yareach on 26. 6. 10..
//

#ifndef AUTH_MANAGER_KEY_PROVIDER_H
#define AUTH_MANAGER_KEY_PROVIDER_H

#include <string>
#include <array>
#include <vector>
#include <cstddef>
#include <filesystem>

namespace auth_manager::auth::key_provider {
    class KeyProvider {
    private:
        void remove_pem_files() const;
    protected:
        const std::string _key_name;
        const std::filesystem::path _file_base;
        const std::filesystem::path _private_key_file_path;
        const std::filesystem::path _public_key_file_path;

        [[nodiscard]] std::array<const std::filesystem::path, 2> key_file_paths() const;

        virtual void free_keys_impl() = 0;

        virtual void generate_keys_impl() = 0;
        virtual void save_keys_impl(const std::string& private_key_path, const std::string& public_key_path) = 0;
        virtual void load_keys_impl(const std::string& private_key_path, const std::string& public_key_path) = 0;

        virtual std::vector<std::byte> sign_impl(const std::string &message) = 0;
        virtual bool verify_impl(const std::string &message, const std::vector<std::byte> &signature) = 0;

        [[nodiscard]] virtual bool is_key_loaded_impl() const = 0;

        [[nodiscard]] virtual std::string export_public_key_impl() const = 0;
    public:
        explicit KeyProvider(const std::string& file_base, const std::string& key_name);

        virtual ~KeyProvider();

        void generate_new_keys();
        void load_keys();
        void remove_keys();

        std::vector<std::byte> sign(const std::string &message);
        bool verify(const std::string& message, const std::vector<std::byte>& signature);

        [[nodiscard]] bool is_key_loaded() const;

        [[nodiscard]] bool pem_files_exist() const;

        [[nodiscard]] std::string_view key_name() const;

        [[nodiscard]] std::filesystem::path file_base() const;
        [[nodiscard]] std::filesystem::path private_key_file_path() const;
        [[nodiscard]] std::filesystem::path public_key_file_path() const;

        [[nodiscard]] std::string export_public_key() const;
    };
}

#endif //AUTH_MANAGER_KEY_PROVIDER_H
