//
// Created by yareach on 26. 6. 8..
//

#ifndef AUTH_MANAGER_JSON_FILE_MANAGER_H
#define AUTH_MANAGER_JSON_FILE_MANAGER_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string_view>
#include "IJsonConverter.h"

namespace auth_manager::core::json {
    template<typename T>
    class JsonFileManager {
    private:
        std::shared_ptr<IJsonConverter<T>> jsonConverter;
    public:
        explicit JsonFileManager(std::shared_ptr<IJsonConverter<T>> jsonConverter);

        T read_from_file(std::string_view file_name) const;

        void write_to_file(std::string_view file_name, T data, unsigned int indent = 0) const;

        void delete_file(std::string_view file_name) const;
    };

    template<typename T>
    JsonFileManager<T>::JsonFileManager(std::shared_ptr<IJsonConverter<T>> jsonConverter): jsonConverter(jsonConverter) {}

    template<typename T>
    T JsonFileManager<T>::read_from_file(const std::string_view file_name) const {
        std::ifstream file((file_name.data()));
        if (!file.is_open()) {
            std::cerr << "JsonUtil::load_json_file failed: " << file_name << std::endl;
            throw std::runtime_error("JsonUtil::load_json_file failed");
        }
        std::stringstream ss;
        ss << file.rdbuf();
        T result = jsonConverter->deserialize(ss.str());
        file.close();
        return result;
    }

    template<typename T>
    void JsonFileManager<T>::write_to_file(const std::string_view file_name, T data, unsigned int indent) const {
        std::ofstream file((file_name.data()));
        if (!file.is_open()) {
            std::cerr << "JsonUtil::write_json_file failed: " << file_name << std::endl;
            throw std::runtime_error("JsonUtil::write_json_file failed");
        }
        const std::string result = jsonConverter->serialize(data, indent);
        file << result;
        file.close();
    }

    template<typename T>
    void JsonFileManager<T>::delete_file(const std::string_view file_name) const {
        if (!std::filesystem::exists(file_name)) {
            std::cerr << "file is not exists" << file_name << std::endl;
            throw std::runtime_error("File is not exists");
        }
        std::filesystem::remove(file_name);
    }
}

#endif //AUTH_MANAGER_JSON_FILE_MANAGER_H
