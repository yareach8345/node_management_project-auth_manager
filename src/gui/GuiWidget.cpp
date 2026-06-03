//
// Created by yareach on 26. 6. 3..
//

#include "gui/GuiWidget.h"

#include <QtWidgets/QLabel>

namespace auth_manager::gui {
    GuiWidget::GuiWidget() {
        _tab_widget.addTab(new QLabel("hello world"), "hello");
        _tab_widget.addTab(new QLabel("bye world"), "bye");

        setCentralWidget(&_tab_widget);
    };
}