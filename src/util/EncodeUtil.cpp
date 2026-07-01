//
// Created by yareach on 26. 6. 30..
//

#include "auth_manager/util/EncodeUtil.h"

#include <openssl/evp.h>

namespace auth_manager::util {
    std::string EncodeUtil::base64_encode(const std::vector<std::byte> &bytes) {
        const long length = bytes.size();

        if (length == 0) return "";

        const size_t encoded_length = 4 * ((length + 2) / 3);
        std::vector<unsigned char> encoded_data(encoded_length + 1);

        const int result_len = EVP_EncodeBlock(encoded_data.data(), reinterpret_cast<const unsigned char *>(&bytes[0]), length);

        return std::string(reinterpret_cast<const char*>(encoded_data.data()), result_len);

    }
}
