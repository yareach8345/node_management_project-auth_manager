//
// Created by yareach on 26. 6. 30..
//

#include "auth_manager/auth/gui/MessageSign.h"

#include <QLabel>

#include "auth_manager/util/EncodeUtil.h"

namespace auth_manager::auth::gui {
    MessageSign::MessageSign(const std::shared_ptr<IKeyService> &key_service, QWidget *parent):
        QWidget(parent),
        _key_service(key_service)
    {
        _output->setReadOnly(true);
        init_layout();
        init_event();
    }

    void MessageSign::init_layout() {
        setLayout(_layout);

        _layout->addWidget(new QLabel("Sign Test"));

        _layout->addLayout(_sign_layout);

        const QPointer input_layout = new QVBoxLayout;
        _sign_layout->addLayout(input_layout);
        const QPointer input_label = new QLabel("Message");
        input_label->setAlignment(Qt::AlignCenter);
        input_layout->addWidget(input_label);
        input_layout->addWidget(_input);

        _sign_layout->addWidget(new QLabel(">>"));

        const QPointer output_layout = new QVBoxLayout;
        _sign_layout->addLayout(output_layout);
        const QPointer output_label = new QLabel("Signed Message(Base 64)");
        output_label->setAlignment(Qt::AlignCenter);
        output_layout->addWidget(output_label);
        output_layout->addWidget(_output);
    }

    void MessageSign::init_event() {
        QObject::connect(_input, &QTextEdit::textChanged, this, &MessageSign::on_input_changed);
    }

    void MessageSign::on_input_changed() {
        const auto input_str = _input->toPlainText().toStdString();

        if (!_key_service->is_key_loaded()) {
            _output->setPlainText("Key is not loaded");
            return;
        }

        const auto output_bytes = _key_service->sign(input_str);

        const std::string output_str = util::EncodeUtil::base64_encode(output_bytes);

        _output->setPlainText(QString::fromStdString(output_str));
    }

    void MessageSign::update_input_enable() {
        _input->setEnabled(_key_service->is_key_loaded());
    }

    void MessageSign::updated() {
        update_input_enable();
    }
}
