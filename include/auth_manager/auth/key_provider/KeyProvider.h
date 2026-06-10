//
// Created by yareach on 26. 6. 10..
//

#ifndef AUTH_MANAGER_KEY_PROVIDER_H
#define AUTH_MANAGER_KEY_PROVIDER_H

#include <string>
#include <array>
#include <vector>

namespace auth_manager::auth::key_provider {
    class KeyProvider {
    protected:
        const std::string _key_name;
        const std::string _file_base;
        const std::string _private_key_file_path;
        const std::string _public_key_file_path;

        [[nodiscard]] std::array<std::string_view, 2> key_file_paths() const;

    public:
        explicit KeyProvider(const std::string& file_base, const std::string& key_name);

        virtual ~KeyProvider();

        virtual void load_keys() = 0;
        virtual void generate_new_keys() = 0;
        virtual void free_keys() = 0;
        void delete_keys();

        virtual std::vector<unsigned char> sign(const std::string &message) = 0;
        virtual bool verify(const std::string& message, const std::vector<unsigned char>& signature) = 0;

        [[nodiscard]] virtual bool is_key_loaded() const = 0;

        [[nodiscard]] bool pem_files_exist() const;

        [[nodiscard]] std::string_view file_base() const;
        [[nodiscard]] std::string_view key_name() const;

        [[nodiscard]] std::string_view private_key_file_path() const;
        [[nodiscard]] std::string_view public_key_file_path() const;

        [[nodiscard]] virtual std::string export_public_key() const = 0;
    };
}

#endif //AUTH_MANAGER_KEY_PROVIDER_H
