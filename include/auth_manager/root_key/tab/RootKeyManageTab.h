//
// Created by yareach on 26. 6. 7..
//

#ifndef AUTH_MANAGER_ROOT_KEY_MANAGE_TAB_H
#define AUTH_MANAGER_ROOT_KEY_MANAGE_TAB_H

#include <QTabWidget>
#include <QtWidgets/QPushButton>

#include "auth_manager/auth/gui/KeyInfoViewer.h"
#include "auth_manager/auth/gui/MessageSign.h"
#include "auth_manager/auth/gui/MessageVerify.h"
#include "auth_manager/auth/service/IKeyService.h"
#include "auth_manager/auth/gui/PublicKeyViewer.h"

namespace auth_manager::root_key {
    using namespace auth_manager;

    class RootKeyManageTab: public QWidget {
        Q_OBJECT
    private:
        QPointer<QVBoxLayout> _main_layout = new QVBoxLayout(this);
        QPointer<auth::gui::KeyInfoViewer> _key_info_viewer;

        QPointer<QHBoxLayout> _buttons_layout = new QHBoxLayout();
        QPointer<QPushButton> _generate_key_button = new QPushButton("Generate Key", this);
        QPointer<QPushButton> _delete_key_button = new QPushButton("Delete Key", this);

        QPointer<QTabWidget> _root_key_task_tap = new QTabWidget(this);

        QPointer<auth::gui::PublicKeyViewer> _public_key_viewer;
        QPointer<auth::gui::MessageSign> _message_sign;
        QPointer<auth::gui::MessageVerify> _message_verify;

        std::shared_ptr<auth::IKeyService> _root_key_service;

        void init_layout();

        void connect_event();

        void on_generate_key_button_clicked();

        void on_delete_key_button_clicked();

        void key_updated();
    public:
        explicit RootKeyManageTab(const std::shared_ptr<auth::IKeyService> &root_key_service);
    };
}

#endif //AUTH_MANAGER_ROOT_KEY_MANAGE_TAB_H
