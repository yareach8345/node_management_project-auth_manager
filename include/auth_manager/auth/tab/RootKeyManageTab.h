//
// Created by yareach on 26. 6. 7..
//

#ifndef AUTH_MANAGER_ROOT_KEY_MANAGE_TAB_H
#define AUTH_MANAGER_ROOT_KEY_MANAGE_TAB_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QPushButton>
#include <QtCore/QPointer>
#include <memory>

#include "auth_manager/auth/service/IKeyService.h"

namespace auth_manager::auth::gui {
    class RootKeyManageTab: public QWidget {
        Q_OBJECT
    private:
        QPointer<QVBoxLayout> _main_layout = new QVBoxLayout(this);
        QPointer<QTextBrowser> _key_info_viewer = new QTextBrowser;

        QPointer<QHBoxLayout> _buttons_layout = new QHBoxLayout;
        QPointer<QPushButton> _generate_key_button = new QPushButton("Generate Key", this);
        QPointer<QPushButton> _delete_key_button = new QPushButton("Delete Key", this);

        std::shared_ptr<IKeyService> _root_key_service;

        void init_layout();

        void connect_event();

        void on_generate_key_button_clicked();

        void on_delete_key_button_clicked();

        void update_key_info_viewer();
    public:
        explicit RootKeyManageTab(const std::shared_ptr<IKeyService> &root_key_service);
    };
}

#endif //AUTH_MANAGER_ROOT_KEY_MANAGE_TAB_H
