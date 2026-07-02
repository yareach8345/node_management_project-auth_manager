//
// Created by yareach on 26. 7. 1..
//

#ifndef AUTH_MANAGER_MESSAGE_VERIFY_H
#define AUTH_MANAGER_MESSAGE_VERIFY_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtCore/QPointer>

#include <memory>

#include "auth_manager/auth/service/IKeyService.h"

namespace auth_manager::auth::gui {
    class MessageVerify : public QWidget {
        Q_OBJECT
    private:
        QPointer<QTextEdit> _sign_input = new QTextEdit(this);
        QPointer<QTextEdit> _message_input = new QTextEdit(this);
        QPointer<QTextEdit> _result_output = new QTextEdit(this);

        QPointer<QPushButton> _verify_button = new QPushButton("verify", this);

        std::shared_ptr<IKeyService> _key_service;

        void init_layout();

        void init_event();

        void on_verify_button_clicked();

        void update_enable();
    public:
        explicit MessageVerify(const std::shared_ptr<IKeyService> &key_service, QWidget* parent = nullptr);

        void updated();
    };
}

#endif //AUTH_MANAGER_MESSAGE_VERIFY_H
