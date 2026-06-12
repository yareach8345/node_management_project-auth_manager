//
// Created by yareach on 26. 6. 12..
//

#include <gtest/gtest.h>
#include "auth_manager/auth/key_provider/KeyProvider.h"

using auth_manager::auth::key_provider::KeyProvider;

class KeyProviderTestImpl: public KeyProvider {
protected:
    void free_keys() override;

    void generate_keys() override;

    void save_keys(const std::string &private_key_path, const std::string &public_key_path) override;

    void load_keys(const std::string &private_key_path, const std::string &public_key_path) override;

    std::vector<std::byte> sign(const std::string &message) override;

    bool verify(const std::string &message, const std::vector<std::byte> &signature) override;

    [[nodiscard]] bool is_key_loaded() const override;

    [[nodiscard]] std::string export_public_key() const override;
};

TEST(KeyProviderTest, )