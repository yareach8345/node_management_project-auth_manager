//
// Created by yareach on 26. 6. 28..
//

#include <utility>

#include "auth_manager/auth/gui/PublicKeyViewer.h"

namespace auth_manager::auth::gui {
    PublicKeyViewer::PublicKeyViewer(std::shared_ptr<IKeyService> _root_key_service, QWidget *parent):
        QWidget(parent),
        _root_key_service(std::move(_root_key_service))
    {
        init_layout();
        update_key_viewer_browser();
    }

    void PublicKeyViewer::init_layout() {
        setLayout(_layout);
        _layout->addWidget(_label);
        _layout->addWidget(_viewer_browser);
    }

    void PublicKeyViewer::update_key_viewer_browser() {
        const std::string public_key = _root_key_service->export_public_key().value_or("Key is not loaded");
        _viewer_browser->setText(public_key.c_str());
    }

    void PublicKeyViewer::updated() {
        update_key_viewer_browser();
    }
}
