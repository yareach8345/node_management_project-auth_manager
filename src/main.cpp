#include <iostream>
#include <QApplication>
#include <yaml-cpp/yaml.h>

#include "auth_manager/auth/config/AuthConfig.h"
#include "auth_manager/auth/key_provider/KeyProvider.h"
#include "auth_manager/auth/key_provider/KeyProviderOpenSSLImpl.h"
#include "auth_manager/auth/service/IKeyService.h"
#include "auth_manager/auth/service/KeyServiceOpenSSLImpl.h"

int main(int argc, char *argv[]) {
    // 테스트용 임시 코드

    YAML::Node config = YAML::LoadFile(CONFIG_FILE_PATH);
    const auth_manager::auth::AuthConfig auth_config(config);

    auto provider = auth_manager::auth::key_provider::KeyProviderOpenSSLImpl(auth_config.file_base(), "root");

    std::cout << provider.file_base() << std::endl;
    std::cout << provider.key_name() << std::endl;
    std::cout << provider.private_key_file_path() << std::endl;
    std::cout << provider.public_key_file_path() << std::endl;

    std::cout << "first : " << (provider.is_key_loaded() ? "true" : "false") << std::endl;
    provider.generate_new_keys();
    std::cout << "second: " << (provider.is_key_loaded() ? "true" : "false") << std::endl;

    const auto signed_h = provider.sign("hello world!");

    std::cout << (provider.verify("hello world!", signed_h) ? "true" : "false") << std::endl;
    std::cout << (provider.verify("hello world", signed_h) ? "true" : "false") << std::endl;

    provider.remove_keys();
    std::cout << "third: " << (provider.is_key_loaded() ? "true" : "false") << std::endl;

    return 0;
    //
    // std::shared_ptr<auth_manager::auth::IKeyService> ssl = std::make_shared<auth_manager::auth::KeyServiceOpenSSLImpl>(auth_config, "root");
    // ssl->generate_new_keys();
    // std::cout << ssl->key_name() << std::endl;
    // std::cout << ssl->export_public_key() << std::endl;
    // const auto signed_h = ssl->sign("hello world!");
    //
    // std::cout << (ssl->verify("hello world!", signed_h) ? "true" : "false") << std::endl;
    // std::cout << (ssl->verify("hello world", signed_h) ? "true" : "false") << std::endl;
    //
    // QApplication a(argc, argv);
    //
    // auth_manager::gui::TabInfo tab1 {
    //     .widget = new QLabel("hello world"),
    //     .tab_name = "hello"
    // };
    //
    // auth_manager::gui::TabInfo tab2 {
    //     .widget = new QLabel("bye world"),
    //     .tab_name = "bye"
    // };
    //
    // auth_manager::gui::TabInfo tab3 {
    //     .widget = new auth_manager::auth::gui::RootKeyManageTab(ssl),
    //     .tab_name = "root_key_manage(test)"
    // };
    //
    // auth_manager::gui::GuiWidget window{{ tab1, tab2, tab3 }};
    // window.setFixedSize(500, 500);
    // window.show();
    //
    // return QApplication::exec();
}
