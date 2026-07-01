//
// Created by yareach on 26. 6. 7..
//

#include "auth_manager/root_key/tab/RootKeyManageTab.h"
#include <QTabWidget>
#include "auth_manager/auth/gui/PublicKeyViewer.h"
#include "auth_manager/auth/mapping/KeysInfoJsonConverter.h"

namespace auth_manager::root_key {
    RootKeyManageTab::RootKeyManageTab(const std::shared_ptr<auth::IKeyService> &root_key_service):
        _key_info_viewer(new auth::gui::KeyInfoViewer(root_key_service, this)),
        _public_key_viewer(new auth::gui::PublicKeyViewer(root_key_service, this)),
        _message_sign(new auth::gui::MessageSign(root_key_service, this)),
        _root_key_service(root_key_service)
    {
        init_layout();
        connect_event();
        update_key_info_viewer();
        update_public_key_viewer();
    }

    void RootKeyManageTab::init_layout() {
        _buttons_layout->addWidget(_generate_key_button);
        _buttons_layout->addWidget(_delete_key_button);
        _main_layout->addLayout(_buttons_layout);

        _main_layout->addWidget(_key_info_viewer);

        _main_layout->addWidget(_root_key_task_tap);
        _root_key_task_tap->addTab(_public_key_viewer, "Public Key Viewer");
        _root_key_task_tap->addTab(_message_sign, "Message Sign Test");

        setLayout(_main_layout);
    }

    void RootKeyManageTab::connect_event() {
        connect(_generate_key_button, &QPushButton::clicked, this, &RootKeyManageTab::on_generate_key_button_clicked);
        connect(_delete_key_button, &QPushButton::clicked, this, &RootKeyManageTab::on_delete_key_button_clicked);
    }

    void RootKeyManageTab::on_generate_key_button_clicked() {
        _root_key_service->generate_new_keys();
        update_key_info_viewer();
        update_public_key_viewer();
    }

    void RootKeyManageTab::on_delete_key_button_clicked() {
        _root_key_service->delete_keys();
        update_key_info_viewer();
        update_public_key_viewer();
    }

    void RootKeyManageTab::update_key_info_viewer() {
        _key_info_viewer->updated();
    }

    void RootKeyManageTab::update_public_key_viewer() {
        _public_key_viewer->updated();
    }
}
