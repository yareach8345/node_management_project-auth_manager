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
        QPointer<QLabel> _test_label = new QLabel;
        QPointer<QTextBrowser> _key_info_viewer = new QTextBrowser;

        std::shared_ptr<IKeyService> _root_key_service;

        void init_layout();
    public:
        explicit RootKeyManageTab(const std::shared_ptr<IKeyService> &root_key_service);
    };
}

#endif //AUTH_MANAGER_ROOT_KEY_MANAGE_TAB_H
