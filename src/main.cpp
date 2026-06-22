#include <iostream>
#include <QApplication>
#include <yaml-cpp/yaml.h>

#include "auth_manager/auth/config/AuthConfig.h"
#include "auth_manager/auth/key_provider/KeyProvider.h"
#include "auth_manager/auth/key_provider/KeyProviderOpenSSLImpl.h"
#include "auth_manager/auth/service/IKeyService.h"
#include "auth_manager/auth/service/KeyServiceImpl.h"
#include "auth_manager/auth/tab/RootKeyManageTab.h"
#include "auth_manager/gui/GuiWidget.h"
#include "auth_manager/gui/TabInfo.h"

int main(int argc, char *argv[]) {
    YAML::Node config = YAML::LoadFile(CONFIG_FILE_PATH);
    const auth_manager::auth::AuthConfig auth_config(config);

    std::unique_ptr<auth_manager::auth::key_provider::KeyProvider> provider = std::make_unique<auth_manager::auth::key_provider::KeyProviderOpenSSLImpl>(auth_config.file_base(), "root");
    auto json_file_manager = auth_manager::core::json::JsonFileManager(auth_config.file_base() + "/root/keys_info.json", auth_manager::auth::KeysInfoJsonConverter::get_instance());

    auth_manager::auth::KeyServiceImpl ssl(std::move(provider), json_file_manager);

    QApplication a(argc, argv);

    auth_manager::gui::TabInfo tab1 {
        .widget = new QLabel("hello world"),
        .tab_name = "hello"
    };

    auth_manager::gui::TabInfo tab2 {
        .widget = new QLabel("bye world"),
        .tab_name = "bye"
    };

    auth_manager::gui::TabInfo tab3 {
        .widget = new auth_manager::auth::gui::RootKeyManageTab(&ssl),
        .tab_name = "root_key_manage(test)"
    };

    auth_manager::gui::GuiWidget window{{ tab1, tab2, tab3 }};
    window.setFixedSize(500, 500);
    window.show();

    return QApplication::exec();
}
