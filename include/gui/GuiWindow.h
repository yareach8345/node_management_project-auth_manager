//
// Created by yareach on 26. 6. 3..
//

#ifndef AUTH_MANAGER_GUIWINDOW_H
#define AUTH_MANAGER_GUIWINDOW_H

#include <QtWidgets/QMainWindow>

namespace auth_manager::gui {
    class GuiWindow : public QMainWindow {
    private:
        QWidget central_widget;
    public:
        explicit GuiWindow();
    };
}

#endif //AUTH_MANAGER_GUIWINDOW_H
