//
// Created by yareach on 26. 6. 4..
//

#ifndef AUTH_MANAGER_TAB_INFO_H
#define AUTH_MANAGER_TAB_INFO_H

#include <QtWidgets/QWidget>
#include <QtCore/QString>
#include <QtCore/QPointer>

namespace auth_manager::gui {
    struct TabInfo {
        QPointer<QWidget> widget;
        QString tab_name;
    };
}

#endif //AUTH_MANAGER_TAB_INFO_H
