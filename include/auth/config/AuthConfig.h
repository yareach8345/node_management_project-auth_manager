//
// Created by yareach on 26. 6. 2..
//

#ifndef AUTH_MANAGER_AUTH_CONFIG_H
#define AUTH_MANAGER_AUTH_CONFIG_H

#include <string>
#include <yaml-cpp/yaml.h>

namespace auth_manager::auth {
    class AuthConfig {
        const std::string _file_base;
    public:
        explicit AuthConfig(YAML::Node &config);

        [[nodiscard]] const std::string& file_base() const;
    };
}

#endif //AUTH_MANAGER_AUTH_CONFIG_H
