#include <iostream>
#include <QApplication>
#include <yaml-cpp/yaml.h>

#include "auth/config/AuthConfig.h"
#include "auth/root_key_manager/RootKeyManagerOpenSSLImpl.h"
#include "gui/GuiWidget.h"

int main(int argc, char *argv[]) {
    YAML::Node config = YAML::LoadFile(CONFIG_FILE_PATH);
    std::cout << CONFIG_FILE_PATH << std::endl;
    std::cout << config["auth"]["file_base"].as<std::string>() << std::endl;
    const auth_manager::auth::AuthConfig auth_config(config);
    std::cout << auth_config.file_base() << std::endl;

    auth_manager::auth::RootKeyManagerOpenSSLImpl openSSL(auth_config);
    openSSL.generate_new_keys();

    QApplication a(argc, argv);

    auth_manager::gui::GuiWidget window;
    window.setFixedSize(500, 500);
    window.show();

    return QApplication::exec();
}
