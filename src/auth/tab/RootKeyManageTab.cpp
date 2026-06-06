//
// Created by yareach on 26. 6. 7..
//

#include "auth_manager/auth/tab/RootKeyManageTab.h"

namespace auth_manager::auth::gui {
    RootKeyManageTab::RootKeyManageTab(const std::shared_ptr<IKeyService> &root_key_service): _root_key_service(root_key_service) {
        init_layout();
    }

    void RootKeyManageTab::init_layout() {
        _main_layout.addWidget(&_test_label);
        _test_label.setText(_root_key_service.get()->export_public_key().c_str());

        _main_layout.addWidget(&_key_info_viewer);
        _key_info_viewer.setText(_root_key_service.get()->keys_info()->to_json().dump(4).c_str());

        setLayout(&_main_layout);
    }
}
