// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holondockwidget.h"
#include "holonabstractwindow.h"
#include "holontitlebar.h"
#include "holonwindowarea.h"
#include "holonwindowmenu.h"
#include <QBoxLayout>
#include <QLoaderTree>
#include <QMainWindow>

class HolonDockWidgetPrivate
{
public:
    HolonAbstractWindow *const window;
};

HolonDockWidget::HolonDockWidget(QMainWindow *parent, HolonAbstractWindow *window)
:   QDockWidget(parent),
    d(*new (&d_storage) HolonDockWidgetPrivate{window})
{
    static_assert (sizeof (d_storage) == sizeof (HolonDockWidgetPrivate));
    static_assert (sizeof (ptrdiff_t) == alignof (HolonDockWidgetPrivate));

    parent->addDockWidget(Qt::LeftDockWidgetArea, this);
    setFeatures(QDockWidget::NoDockWidgetFeatures);
}

HolonDockWidget::HolonDockWidget(HolonDesktop *desktop, HolonWindowArea *area, QMainWindow *parent)
:   HolonDockWidget(parent)
{
    setTitleBarWidget(new HolonTitleBar(desktop, this));
    QWidget *widget = new QWidget(this);
    {
        QVBoxLayout *l = new QVBoxLayout(widget);
        {
            widget->setLayout(l);
            HolonWindowMenu *menu;
            l->addWidget(menu = new HolonWindowMenu(desktop, widget), 0, Qt::AlignCenter);
            connect(menu, &HolonWindowMenu::triggered, this, [area](HolonAbstractWindow *window)
            {
                QStringList to = area->section();
                to.append(qAsConst(window)->section().last());
                area->tree()->copy(window->section(), to);
            });
        }
        setWidget(widget);
    }
}

HolonDockWidget::HolonDockWidget(HolonDesktop *desktop,
                                 QMainWindow *parent,
                                 HolonAbstractWindow *window,
                                 HolonWindowAreaPrivate *d)
:   HolonDockWidget(parent, window)
{
    setTitleBarWidget(new HolonTitleBar(desktop, this, window, d));
    setWidget(window->widget());

    if (window->isCurrent())
        widget()->setFocus();
}

HolonAbstractWindow *HolonDockWidget::window() const
{
    return d.window;
}
