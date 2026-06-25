//
// Created by yareach on 26. 6. 7..
//

#include "auth_manager/root_key/tab/RootKeyManageTab.h"
#include <QTabWidget>
#include "auth_manager/auth/mapping/KeysInfoJsonConverter.h"

namespace auth_manager::root_key {
    RootKeyManageTab::RootKeyManageTab(const std::shared_ptr<auth::IKeyService> &root_key_service):
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
        _public_key_viewer->setLayout(_public_key_viewer_layout);
        _public_key_viewer_layout->addWidget(_public_key_viewer_label);
        _public_key_viewer_layout->addWidget(_public_key_viewer_browser);

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
        const std::string keys_info_string = _root_key_service->keys_info()
            .transform([](const auth::KeysInfo& info) {
                return auth::KeysInfoJsonConverter::get_instance()->serialize(info, 4);
            }).value_or("Key is not loaded");
        _key_info_viewer->setText(keys_info_string.c_str());
    }

    void RootKeyManageTab::update_public_key_viewer() {
        const std::string public_key = _root_key_service->export_public_key().value_or("Key is not loaded");
        _public_key_viewer_browser->setText(public_key.c_str());
    }
}
