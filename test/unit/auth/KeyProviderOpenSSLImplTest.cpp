//
// Created by yareach on 26. 6. 13..
//

#include <gtest/gtest.h>
#include <filesystem>
#include <vector>
#include <algorithm>
#include "auth_manager/auth/key_provider/KeyProviderOpenSSLImpl.h"
#include "auth_manager/auth/util/OpenSSLUtil.h"
#include "auth_manager/util/FileUtil.h"

using auth_manager::auth::key_provider::KeyProviderOpenSSLImpl;
using auth_manager::util::FileUtil;
using auth_manager::auth::util::OpenSSLUtil;

class KeyProviderOpenSSLImplTest : public ::testing::Test {
private:
    void clear_file() const {
        FileUtil::remove_all_if_exists(test_file_base);
    }
protected:
    const std::filesystem::path test_file_base = "./test_datas/test_keys";
    const std::filesystem::path test_key_name = "test";

    std::unique_ptr<KeyProviderOpenSSLImpl> provider;

    std::unique_ptr<KeyProviderOpenSSLImpl> generate_test_provider() {
        return std::make_unique<KeyProviderOpenSSLImpl>(
            test_file_base,
            test_key_name
        );
    }

    void SetUp() override {
        clear_file();

        provider = generate_test_provider();
    }

    void TearDown() override {
        clear_file();
    }
};

TEST_F(KeyProviderOpenSSLImplTest, generate_new_keys) {
    provider->generate_new_keys();

    ASSERT_TRUE(FileUtil::is_exists(test_file_base / test_key_name));

    const std::vector<std::filesystem::path> test_key_paths(
        std::filesystem::directory_iterator(test_file_base / test_key_name),
        std::filesystem::directory_iterator{}
    );

    ASSERT_EQ(test_key_paths.size(), 2);
    ASSERT_TRUE(std::ranges::find(test_key_paths, test_file_base / test_key_name / "private_key.pem") != test_key_paths.end());
    ASSERT_TRUE(std::ranges::find(test_key_paths, test_file_base / test_key_name / "public_key.pem") != test_key_paths.end());
}

TEST_F(KeyProviderOpenSSLImplTest, remove_keys) {
    provider->generate_new_keys();

    // 파일이 있을 때 : 키가 로드되어있으며, 파일이 존재함
    ASSERT_TRUE(provider->is_key_loaded());
    ASSERT_TRUE(provider->pem_files_exist());

    // remove_keys
    provider->remove_keys();

    // 파일 삭제 후 결과 : 키가 로드되어있지 않으며, 파일이 존재하지 않음
    ASSERT_FALSE(provider->is_key_loaded());
    ASSERT_FALSE(provider->pem_files_exist());
}

TEST_F(KeyProviderOpenSSLImplTest, load_keys) {
    // 키 생성 전에 생성된 새 프로바이더
    const auto test_provider = generate_test_provider();

    // 키 생성
    provider->generate_new_keys();

    // 로드가 되어있지 않은 상태
    ASSERT_FALSE(test_provider->is_key_loaded());

    // 키 로드
    test_provider->load_keys();

    // 로드가 완료됨
    ASSERT_TRUE(test_provider->is_key_loaded());
}

TEST_F(KeyProviderOpenSSLImplTest, sign_and_verify) {
    const std::string message = "Hello, World!";

    provider->generate_new_keys();

    const auto signed_message = provider->sign(message);

    ASSERT_TRUE(provider->verify(message, signed_message));
    ASSERT_FALSE(provider->verify("bye world!", signed_message));
}

TEST_F(KeyProviderOpenSSLImplTest, is_key_loaded) {
    // 객체 생성시 pem 파일이 없을 경우 : 로딩에 실패하여 false
    ASSERT_FALSE(provider->is_key_loaded());

    provider->generate_new_keys();

    // 키 생성 후 : true
    ASSERT_TRUE(provider->is_key_loaded());
    // 객체 생성시 pem 파일이 있는 경우 : 로딩에 성공하여 true
    ASSERT_TRUE(generate_test_provider()->is_key_loaded());

    provider->remove_keys();

    // 키 삭제 후 : false
    ASSERT_FALSE(provider->is_key_loaded());
}

TEST_F(KeyProviderOpenSSLImplTest, pem_files_exists) {
    // 키 생성 전
    ASSERT_FALSE(provider->pem_files_exist());
    ASSERT_FALSE(generate_test_provider()->pem_files_exist());

    // 키 생성
    provider->generate_new_keys();
    ASSERT_TRUE(provider->pem_files_exist());
    ASSERT_TRUE(generate_test_provider()->pem_files_exist());

    // 삭제
    provider->remove_keys();
    ASSERT_FALSE(provider->pem_files_exist());
    ASSERT_FALSE(generate_test_provider()->pem_files_exist());
}

TEST_F(KeyProviderOpenSSLImplTest, key_name) {
    ASSERT_EQ(provider->key_name(), test_key_name);
}

TEST_F(KeyProviderOpenSSLImplTest, file_base) {
    ASSERT_EQ(provider->file_base(), test_file_base / test_key_name);
}

TEST_F(KeyProviderOpenSSLImplTest, private_key_file_path) {
    ASSERT_EQ(provider->private_key_file_path(), test_file_base / test_key_name / "private_key.pem");
}

TEST_F(KeyProviderOpenSSLImplTest, public_key_file_path) {
    ASSERT_EQ(provider->public_key_file_path(), test_file_base / test_key_name / "public_key.pem");
}

TEST_F(KeyProviderOpenSSLImplTest, export_public_key) {
    provider->generate_new_keys();
    const auto public_key_str_from_provider = provider->export_public_key();

    const auto evp_pub_key = OpenSSLUtil::read_evp_public_key(test_file_base / test_key_name / "public_key.pem");
    const auto public_key_str_evp_pkey_t = OpenSSLUtil::export_public_key(evp_pub_key.get());

    ASSERT_EQ(public_key_str_from_provider, public_key_str_evp_pkey_t);
}

TEST_F(KeyProviderOpenSSLImplTest, export_public_key_when_public_key_is_not_generate) {
    ASSERT_THROW(
        auto _ = provider->export_public_key(),
        std::runtime_error
    );
}