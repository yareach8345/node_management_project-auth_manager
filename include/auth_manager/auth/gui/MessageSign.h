//
// Created by yareach on 26. 6. 30..
//

#ifndef AUTH_MANAGER_MESSAGE_SIGN_H
#define AUTH_MANAGER_MESSAGE_SIGN_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPushButton>
#include <QtCore/QPointer>

#include <memory>

#include "auth_manager/auth/service/IKeyService.h"

namespace auth_manager::auth::gui {
    class MessageSign : public QWidget {
        Q_OBJECT
    private:
        QPointer<QVBoxLayout> _layout = new QVBoxLayout;
        QPointer<QHBoxLayout> _sign_layout = new QHBoxLayout;
        QPointer<QTextEdit> _input = new QTextEdit(this);
        QPointer<QTextEdit> _output = new QTextEdit(this);

        std::shared_ptr<IKeyService> _key_service;

        void init_layout();

        void init_event();

        void on_input_changed();

        void update_input_enable();
    public:
        explicit MessageSign(const std::shared_ptr<IKeyService> &key_service, QWidget *parent = nullptr);

        void updated();
    };
}

#endif //AUTH_MANAGER_MESSAGE_SIGN_H
