// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holondockwidget.h"
#include "holonabstracttask.h"
#include "holonabstractwindow.h"
#include "holontitlebar.h"
#include "holonwindowarea.h"
#include "holonwindowarea_p.h"
#include "holonwindowmenu.h"
#include <QBoxLayout>
#include <QLoaderTree>
#include <QMainWindow>

class HolonDockWidgetPrivate
{
public:
    HolonDockWidget *const q_ptr;
    HolonAbstractWindow *const window;
    HolonTitleBar *const titleBar;

    HolonDockWidgetPrivate(HolonDockWidget *q,
                           HolonDesktop *desktop,
                           QMainWindow *parent,
                           HolonAbstractWindow *w = nullptr,
                           HolonWindowAreaPrivate *d = nullptr)
    :   q_ptr(q),
        window(w),
        titleBar(new HolonTitleBar(desktop, q, w, d))
    {
        q_ptr->setTitleBarWidget(titleBar);
        q_ptr->setFeatures(QDockWidget::NoDockWidgetFeatures);
        parent->addDockWidget(Qt::LeftDockWidgetArea, q_ptr);
    }
};

HolonDockWidget::HolonDockWidget(HolonDesktop *desktop,
                                 QMainWindow *parent,
                                 HolonWindowArea *area)
:   QDockWidget(parent),
    d_ptr(this, desktop, parent)
{
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
:   QDockWidget(parent),
    d_ptr(this, desktop, parent, window, d)
{
    setWidget(window->widget());

    if (HolonAbstractTask *task = window->task())
        if (task->isCurrent() && window->isCurrent())
            widget()->setFocus();
}

HolonDockWidget::~HolonDockWidget()
{ }

HolonTitleBar *HolonDockWidget::titleBar() const
{
    return d_ptr->titleBar;
}

HolonAbstractWindow *HolonDockWidget::window() const
{
    return d_ptr->window;
}
