//
// Created by yareach on 26. 6. 30..
//

#ifndef AUTH_MANAGER_ENCODE_UTIL_H
#define AUTH_MANAGER_ENCODE_UTIL_H

#include <string>
#include <vector>

namespace auth_manager::util {
    class EncodeUtil {
    public:
        static std::string base64_encode(const std::vector<std::byte> &bytes);

        static std::vector<std::byte> base64_decode(const std::string &encoded_string);
    };
}

#endif //AUTH_MANAGER_ENCODE_UTIL_H
