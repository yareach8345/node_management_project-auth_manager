//
// Created by yareach on 26. 7. 1..
//

#include "auth_manager/auth/gui/MessageVerify.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>

#include "auth_manager/util/EncodeUtil.h"

namespace auth_manager::auth::gui {
    MessageVerify::MessageVerify(const std::shared_ptr<IKeyService> &key_service, QWidget *parent):
        QWidget(parent),
        _key_service(key_service)
    {
        init_layout();
        init_event();

        _result_output->setReadOnly(true);
    }

    void MessageVerify::init_layout() {
        const QPointer layout = new QVBoxLayout();
        this->setLayout(layout);

        layout->addWidget(new QLabel("Verify Test"));

        const QPointer test_layout = new QHBoxLayout;
        layout->addLayout(test_layout);

        const QPointer sign_input_layout = new QVBoxLayout;
        test_layout->addLayout(sign_input_layout);
        sign_input_layout->addWidget(new QLabel("Sign"));
        sign_input_layout->addWidget(_sign_input);

        const QPointer message_input_layout = new QVBoxLayout;
        test_layout->addLayout(message_input_layout);
        message_input_layout->addWidget(new QLabel("Message"));
        message_input_layout->addWidget(_message_input);

        test_layout->addWidget(new QLabel(">>"));

        const QPointer result_layout = new QVBoxLayout;
        test_layout->addLayout(result_layout);
        result_layout->addWidget(new QLabel("Result"));
        result_layout->addWidget(_result_output);

        layout->addWidget(_verify_button);
    }

    void MessageVerify::init_event() {
        QObject::connect(_verify_button, &QPushButton::clicked, this, &MessageVerify::on_verify_button_clicked);
    }

    void MessageVerify::update_enable() {
        const bool is_key_loaded = _key_service->is_key_loaded();
        _sign_input->setEnabled(is_key_loaded);
        _message_input->setEnabled(is_key_loaded);
        _result_output->setEnabled(is_key_loaded);
        _verify_button->setEnabled(is_key_loaded);
    }

    void MessageVerify::updated() {
        update_enable();
    }

    void MessageVerify::on_verify_button_clicked() {
        const auto sign = util::EncodeUtil::base64_decode(_sign_input->toPlainText().toStdString());
        const auto message = _message_input->toPlainText().toStdString();

        const auto result = _key_service->verify(message, sign);

        _result_output->setText(QString::fromStdString(result ? "true" : "false"));
    }
}