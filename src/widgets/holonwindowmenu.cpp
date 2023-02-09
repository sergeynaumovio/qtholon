// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindowmenu.h"
#include "holondesktop.h"
#include "holonwindow.h"
#include <QBoxLayout>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>

class WindowButton : public QPushButton
{
public:
    WindowButton(HolonDesktop *desktop, HolonWindow *window, HolonWindowMenu *parent)
    :   QPushButton(parent)
    {
        setIcon(window->icon());
        setFixedHeight(desktop->titleBarHeight());
        setFixedWidth(desktop->menuWidth());
        setStyleSheet(desktop->buttonStyleSheet() + "QPushButton { text-align:left; padding-left: 7px; }");
        setText(window->title());

        connect(this, &QPushButton::clicked, this, [window, parent]() { emit parent->triggered(window); });
    }
};

void HolonWindowMenu::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

HolonWindowMenu::HolonWindowMenu(HolonDesktop *desktop, QWidget *parent)
:   QWidget(parent)
{
    int px = desktop->menuBorder();
    using Layout = QVBoxLayout;
    Layout *l = new Layout(this);
    setLayout(l);
    {
        layout()->setContentsMargins({px, px, px, px});
        layout()->setSpacing(0);

        for (HolonWindow *window : desktop->windows())
        {
            if (window->areas().testFlag(HolonWindow::Sidebar))
                l->addWidget(new WindowButton(desktop, window, this), 0, Qt::AlignHCenter);
        }
    }
    setFixedWidth(desktop->menuWidth() + px * 2);
    setStyleSheet(desktop->menuStyleSheet());
}
