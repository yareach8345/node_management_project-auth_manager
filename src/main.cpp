#include <iostream>
#include <QApplication>
#include <QPushButton>
#include <yaml-cpp/yaml.h>

#include "auth/config/AuthConfig.h"
#include "auth/root_key_manager/RootKeyManagerOpenSSLImpl.h"

int main(int argc, char *argv[]) {
    YAML::Node config = YAML::LoadFile(CONFIG_FILE_PATH);
    std::cout << CONFIG_FILE_PATH << std::endl;
    std::cout << config["auth"]["file_base"].as<std::string>() << std::endl;
    const auth_manager::auth::AuthConfig auth_config(config);
    std::cout << auth_config.file_base() << std::endl;

    auth_manager::auth::RootKeyManagerOpenSSLImpl openSSL(auth_config);

    QApplication a(argc, argv);

    QPushButton button("button", nullptr);
    auto update_button_title = [&] {
        button.setText(openSSL.is_key_loaded() ? "delete keys" : "generate keys");
    };
    update_button_title();

    QObject::connect(&button, &QPushButton::clicked, [&] {
        if (openSSL.is_key_loaded()) {
            openSSL.delete_keys();
            update_button_title();
        } else {
            openSSL.update_keys();
            update_button_title();
        }
    });

    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}