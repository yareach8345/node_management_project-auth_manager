//
// Created by yareach on 26. 6. 12..
//

#ifndef AUTH_MANAGER_FILE_UTIL_H
#define AUTH_MANAGER_FILE_UTIL_H

#include <string>
#include <filesystem>

namespace auth_manager::util {
    class FileUtil {
    public:
        static bool is_exists(const std::filesystem::path &path);

        static std::string current_path();

        static void remove(const std::filesystem::path &path);

        static void remove_all(const std::filesystem::path &path);

        static void remove_if_exists(const std::filesystem::path &path);

        static void remove_all_if_exists(const std::filesystem::path &path);

        static void create_directory(const std::filesystem::path &directory_path);
    };
}

#endif