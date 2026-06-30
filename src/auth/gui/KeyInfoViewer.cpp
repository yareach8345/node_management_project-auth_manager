//
// Created by yareach on 26. 6. 29..
//

#include "auth_manager/auth/gui/KeyInfoViewer.h"

#include "auth_manager/auth/mapping/KeysInfoJsonConverter.h"

namespace auth_manager::auth::gui {
    KeyInfoViewer::KeyInfoViewer(std::shared_ptr<IKeyService> root_key_service, QWidget *parent):
        _root_key_service(root_key_service),
        QWidget(parent)
    {
        init_layout();
        update_key_viewer_browser();
    }

    void KeyInfoViewer::init_layout() {
        setLayout(_layout);
        _layout->addWidget(_key_info_viewer);
    }

    void KeyInfoViewer::update_key_viewer_browser() {
        const std::string keys_info_string = _root_key_service->keys_info()
            .transform([](const auth::KeysInfo& info) {
                return auth::KeysInfoJsonConverter::get_instance()->serialize(info, 4);
            }).value_or("Key is not loaded");
        _key_info_viewer->setText(keys_info_string.c_str());
    }

    void KeyInfoViewer::updated() {
        update_key_viewer_browser();
    }
}
