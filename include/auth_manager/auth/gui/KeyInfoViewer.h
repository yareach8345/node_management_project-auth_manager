//
// Created by yareach on 26. 6. 29..
//

#ifndef AUTH_MANAGER_KEY_INFO_VIEWER_H
#define AUTH_MANAGER_KEY_INFO_VIEWER_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtCore/QPointer>
#include "auth_manager/auth/service/IKeyService.h"

namespace auth_manager::auth::gui {
    class KeyInfoViewer : public QWidget {
        Q_OBJECT
    private:
        std::shared_ptr<IKeyService> _root_key_service;

        QPointer<QVBoxLayout> _layout = new QVBoxLayout;
        QPointer<QTextBrowser> _key_info_viewer = new QTextBrowser(this);

        void init_layout();

        void update_key_viewer_browser();
    public:
        KeyInfoViewer(std::shared_ptr<IKeyService> root_key_service, QWidget *parent);

        void updated();
    };
}

#endif //AUTH_MANAGER_KEY_INFO_VIEWER_H
