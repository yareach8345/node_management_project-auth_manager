//
// Created by yareach on 26. 6. 12..
//

#include "auth_manager/util/FileUtil.h"

#include <filesystem>

namespace auth_manager::util {
    bool FileUtil::is_exists(const std::filesystem::path &path) {
        return std::filesystem::exists(path);
    }

    std::string FileUtil::current_path() {
        return std::filesystem::current_path();
    }

    void FileUtil::remove(const std::filesystem::path &path) {
        std::filesystem::remove(path);
    }

    void FileUtil::remove_if_exists(const std::filesystem::path &path) {
        if (std::filesystem::exists(path)) {
            std::filesystem::remove(path);
        }
    }

    void FileUtil::remove_all(const std::filesystem::path &path) {
        std::filesystem::remove_all(path);
    }

    void FileUtil::remove_all_if_exists(const std::filesystem::path &path) {
        if (std::filesystem::exists(path)) {
            std::filesystem::remove_all(path);
        }
    }

    void FileUtil::create_directory(const std::filesystem::path &directory_path) {
        std::filesystem::create_directory(directory_path);
    }
}
