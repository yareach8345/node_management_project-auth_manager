#include <iostream>
#include <QApplication>
#include <QLabel>
#include <yaml-cpp/yaml.h>
#include "auth_manager/gui/TabInfo.h"

#include "auth_manager/auth/config/AuthConfig.h"
#include "auth_manager/auth/service/IKeyService.h"
#include "auth_manager/auth/service/KeyServiceOpenSSLImpl.h"
#include "auth_manager/gui/GuiWidget.h"

int main(int argc, char *argv[]) {
    YAML::Node config = YAML::LoadFile(CONFIG_FILE_PATH);
    const auth_manager::auth::AuthConfig auth_config(config);

    std::unique_ptr<auth_manager::auth::IKeyService> ssl = std::make_unique<auth_manager::auth::KeyServiceOpenSSLImpl>(auth_config, "root");
    ssl->generate_new_keys();
    std::cout << ssl->key_name() << std::endl;
    std::cout << ssl->export_public_key() << std::endl;
    const auto signed_h = ssl->sign("hello world!");

    std::cout << (ssl->verify("hello world!", signed_h) ? "true" : "false") << std::endl;
    std::cout << (ssl->verify("hello world", signed_h) ? "true" : "false") << std::endl;

    QApplication a(argc, argv);

    auth_manager::gui::TabInfo tab1 {
        .widget = new QLabel("hello world"),
        .tab_name = "hello"
    };

    auth_manager::gui::TabInfo tab2 {
        .widget = new QLabel("bye world"),
        .tab_name = "bye"
    };

    auth_manager::gui::GuiWidget window{ { tab1, tab2 } };
    window.setFixedSize(500, 500);
    window.show();

    return QApplication::exec();
}
