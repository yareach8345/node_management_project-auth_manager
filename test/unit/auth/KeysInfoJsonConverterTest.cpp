//
// Created by yareach on 26. 6. 16..
//

#include <gtest/gtest.h>
#include "auth_manager/auth/data/KeysInfo.h"
#include "auth_manager/auth/mapping/KeysInfoJsonConverter.h"

using namespace auth_manager::auth;

class KeysInfoJsonConverterTest : public ::testing::Test {};

std::shared_ptr<auth_manager::core::json::JsonConverter<KeysInfo>> converter = KeysInfoJsonConverter::get_instance();

const KeysInfo test_keys_info { .created_at = "2026-06-16 17:25:00" };
std::string test_keys_info_json_string_with_index =
    "{\n"
    "    \"created_at\": \"2026-06-16 17:25:00\"\n"
    "}";

std::string test_keys_info_json_string_without_index =
    R"({"created_at":"2026-06-16 17:25:00"})";

void ASSERT_KEYS_INFO_EQ(const KeysInfo& expected, const KeysInfo& actual) {
    ASSERT_EQ(expected.created_at, actual.created_at);
}

TEST_F(KeysInfoJsonConverterTest, serialize_with_index) {
    const auto serialize_result = converter->serialize(test_keys_info, 4);

    ASSERT_EQ(serialize_result, test_keys_info_json_string_with_index);
}

TEST_F(KeysInfoJsonConverterTest, serialize_without_index) {
    const auto serialize_result = converter->serialize(test_keys_info);

    ASSERT_EQ(serialize_result, test_keys_info_json_string_without_index);
}

TEST_F(KeysInfoJsonConverterTest, deserialize_with_index) {
    const auto deserialize_result = converter->deserialize(test_keys_info_json_string_with_index);

    ASSERT_KEYS_INFO_EQ(test_keys_info, deserialize_result);
}

TEST_F(KeysInfoJsonConverterTest, deserialize_without_index) {
    const auto deserialize_result = converter->deserialize(test_keys_info_json_string_without_index);

    ASSERT_KEYS_INFO_EQ(test_keys_info, deserialize_result);
}
