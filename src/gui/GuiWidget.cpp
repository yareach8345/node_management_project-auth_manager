//
// Created by yareach on 26. 6. 3..
//

#include "auth_manager/gui/GuiWidget.h"
#include <algorithm>
#include <QtWidgets/QLabel>

namespace auth_manager::gui {
    GuiWidget::GuiWidget() {
        setCentralWidget(&_tab_widget);
    };

    GuiWidget::GuiWidget(const std::vector<TabInfo> &tabs): _tabs(tabs) {
        std::ranges::for_each(_tabs, [this](const TabInfo& tab_info) {
            this->_tab_widget.addTab(tab_info.widget, tab_info.tab_name);
        });

        setCentralWidget(&_tab_widget);
    }

    void GuiWidget::add_tab(const TabInfo &tab) {
        this->_tabs.push_back(tab);
        this->_tab_widget.addTab(tab.widget, tab.tab_name);
    }
}