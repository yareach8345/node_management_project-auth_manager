//
// Created by yareach on 26. 6. 28..
//

#ifndef AUTH_MANAGER_PUBLIC_KEY_VIEWER_H
#define AUTH_MANAGER_PUBLIC_KEY_VIEWER_H

#include <QLabel>
#include <QPointer>
#include <QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTextBrowser>

#include "auth_manager/auth/service/IKeyService.h"

namespace auth_manager::auth::gui {
    class PublicKeyViewer : public QWidget {
        Q_OBJECT
    private:
        QPointer<QVBoxLayout> _layout = new QVBoxLayout();
        QPointer<QLabel> _label = new QLabel("Public Key", this);
        QPointer<QTextBrowser> _viewer_browser = new QTextBrowser(this);

        std::shared_ptr<IKeyService> _root_key_service;

        void init_layout();

        void update_key_viewer_browser();

    public:
        explicit PublicKeyViewer(std::shared_ptr<IKeyService> _root_key_service, QWidget* parent = nullptr);

        void updated();
    };
}

#endif //AUTH_MANAGER_PUBLIC_KEY_VIEWER_H
