//
// Created by yareach on 26. 6. 3..
//

#ifndef AUTH_MANAGER_GUI_WIDGET_H
#define AUTH_MANAGER_GUI_WIDGET_H

#include <QtWidgets/QMainWindow>
#include <QTabWidget>

namespace auth_manager::gui {
    class GuiWidget : public QMainWindow {
    private:
        QTabWidget _tab_widget;
    public:
        explicit GuiWidget();
    };
}

#endif //AUTH_MANAGER_GUI_WIDGET_H
