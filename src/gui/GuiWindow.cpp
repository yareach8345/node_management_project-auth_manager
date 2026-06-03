//
// Created by yareach on 26. 6. 3..
//

#include "gui/GuiWindow.h"

#include <QtWidgets/QLabel>
#include <QVBoxLayout>

namespace auth_manager::gui {
    GuiWindow::GuiWindow(): central_widget(this) {
        const auto label = new QLabel("hello world");
        const auto box_layout = new QVBoxLayout;

        central_widget.setLayout(box_layout);
        box_layout->addWidget(label);
    };
}