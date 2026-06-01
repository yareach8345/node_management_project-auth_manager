#include <QApplication>
#include <QPushButton>

#include "auth/RootKeyManagerOpenSSLImpl.h"

int main(int argc, char *argv[]) {
    auth_manager::auth::RootKeyManagerOpenSSLImpl openSSL;

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