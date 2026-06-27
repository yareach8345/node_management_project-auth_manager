#include <iostream>
#include <QApplication>
#include <yaml-cpp/yaml.h>

#include "auth_manager/auth/config/AuthConfig.h"
#include "auth_manager/auth/key_provider/KeyProvider.h"
#include "auth_manager/auth/service/IKeyService.h"
#include "auth_manager/root_key/tab/RootKeyManageTab.h"
#include "auth_manager/gui/GuiWidget.h"
#include "auth_manager/gui/TabInfo.h"
#include "auth_manager/root_key/factory/root_key_service_factory.h"

int main(int argc, char *argv[]) {
    YAML::Node config = YAML::LoadFile(CONFIG_FILE_PATH);
    const auth_manager::auth::AuthConfig auth_config(config);

    const std::shared_ptr<auth_manager::auth::IKeyService> root_key_service(auth_manager::root_key::generate_root_key_service(auth_config));

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
        .widget = new auth_manager::root_key::RootKeyManageTab(root_key_service),
        .tab_name = "root_key_manage(test)"
    };

    auth_manager::gui::GuiWidget window{{ tab1, tab2, tab3 }};
    window.setFixedSize(500, 500);
    window.show();

    return QApplication::exec();
}
