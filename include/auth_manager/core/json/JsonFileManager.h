//
// Created by yareach on 26. 6. 8..
//

#ifndef AUTH_MANAGER_JSON_FILE_MANAGER_H
#define AUTH_MANAGER_JSON_FILE_MANAGER_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include "JsonConverter.h"
#include "auth_manager/util/FileUtil.h"

namespace auth_manager::core::json {
    template<typename T>
    class JsonFileManager {
    private:
        std::filesystem::path _file_path;

        std::shared_ptr<JsonConverter<T>> jsonConverter;
    public:
        explicit JsonFileManager(std::string file_path, std::shared_ptr<JsonConverter<T>> jsonConverter);

        [[nodiscard]] bool exists() const;

        T read_from_file() const;

        void write_to_file(T data, unsigned int indent = 0) const;

        void delete_file() const;

        std::filesystem::path file_path() const;
    };

    template<typename T>
    JsonFileManager<T>::JsonFileManager(std::string file_path, std::shared_ptr<JsonConverter<T>> jsonConverter): _file_path(std::move(file_path)), jsonConverter(jsonConverter) {}

    template<typename T>
    bool JsonFileManager<T>::exists() const {
        return std::filesystem::exists(_file_path);
    }

    template<typename T>
    T JsonFileManager<T>::read_from_file() const {
        std::ifstream file(_file_path);
        if (!file.is_open()) {
            std::cerr << "JsonUtil::load_json_file failed: " << _file_path << std::endl;
            throw std::runtime_error("JsonUtil::load_json_file failed");
        }

        std::stringstream ss;
        ss << file.rdbuf();
        T result = jsonConverter->deserialize(ss.str());
        file.close();

        return result;
    }

    template<typename T>
    void JsonFileManager<T>::write_to_file(T data, unsigned int indent) const {
        std::ofstream file(_file_path);
        if (!file.is_open()) {
            std::cerr << "JsonUtil::write_json_file failed: " << _file_path << std::endl;
            throw std::runtime_error("JsonUtil::write_json_file failed");
        }
        const std::string result = jsonConverter->serialize(data, indent);
        file << result;
        file.close();
    }

    template<typename T>
    void JsonFileManager<T>::delete_file() const {
        util::FileUtil::remove_if_exists(_file_path);
    }

    template<typename T>
    std::filesystem::path JsonFileManager<T>::file_path() const {
        return _file_path;
    }
}

#endif //AUTH_MANAGER_JSON_FILE_MANAGER_H
