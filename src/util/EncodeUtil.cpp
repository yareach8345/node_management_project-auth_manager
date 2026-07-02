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

    std::vector<std::byte> EncodeUtil::base64_decode(const std::string &encoded_string) {
        BIO* b64 = BIO_new(BIO_f_base64());
        BIO* bio = BIO_new_mem_buf(encoded_string.data(), encoded_string.size());

        // Base64 문자열에 줄바꿈이 없을 경우
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

        bio = BIO_push(b64, bio);

        std::vector<std::byte> output(encoded_string.size());

        const int decodedLen = BIO_read(bio, output.data(), output.size());

        BIO_free_all(bio);

        if (decodedLen < 0)
            return {};

        output.resize(decodedLen);
        return output;
    }
}
