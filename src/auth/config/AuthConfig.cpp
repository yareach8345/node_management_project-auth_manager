//
// Created by yareach on 26. 6. 2..
//

#include "auth_manager/auth/config/AuthConfig.h"

namespace auth_manager::auth {
    AuthConfig::AuthConfig(YAML::Node &config):
        _file_base(config["auth"]["file_base"].as<std::string>())
    {};

    const std::string &AuthConfig::file_base() const {
        return _file_base;
    }
}
