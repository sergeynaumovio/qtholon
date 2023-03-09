// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindowmenu.h"
#include "holonabstractwindow.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include "holonwindowarea.h"
#include <QBoxLayout>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>

class WindowButton : public QPushButton
{
public:
    WindowButton(HolonDesktop *desktop, HolonAbstractWindow *window, HolonWindowMenu *parent)
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

template<typename T>
T HolonWindowMenu::findParent()
{
    T t{};
    for (QObject *o = parent(); o && !(t = qobject_cast<T>(o)); o = o->parent());

    return t;
}

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
    int px = desktop->menuBorderWidth();

    QVBoxLayout *l = new QVBoxLayout(this);
    {
        setLayout(l);
        layout()->setContentsMargins({px, px, px, px});
        layout()->setSpacing(0);

        if (HolonWindowArea *windowArea = findParent<HolonWindowArea *>())
        {
            HolonAbstractWindow::Area menuWindowArea = HolonAbstractWindow::Central;

            if (qobject_cast<HolonSidebar *>(windowArea))
                menuWindowArea = HolonAbstractWindow::Sidebar;

            for (HolonAbstractWindow *window : desktop->windows())
                if (window->areas().testAnyFlag(menuWindowArea))
                    l->addWidget(new WindowButton(desktop, window, this), 0, Qt::AlignHCenter);
        }
    }
    setFixedWidth(desktop->menuWidth() + px * 2);
    setStyleSheet(desktop->menuStyleSheet());
}
