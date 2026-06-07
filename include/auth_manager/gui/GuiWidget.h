//
// Created by yareach on 26. 6. 3..
//

#ifndef AUTH_MANAGER_GUI_WIDGET_H
#define AUTH_MANAGER_GUI_WIDGET_H

#include <QtWidgets/QMainWindow>
#include <QTabWidget>
#include <QPointer>
#include "TabInfo.h"

namespace auth_manager::gui {
    class GuiWidget : public QMainWindow {
    private:
        QPointer<QTabWidget> _tab_widget = new QTabWidget;

        std::vector<TabInfo> _tabs;
    public:
        explicit GuiWidget();

        explicit GuiWidget(const std::vector<TabInfo> &tabs);

        void add_tab(const TabInfo &tab);
    };
}

#endif //AUTH_MANAGER_GUI_WIDGET_H
