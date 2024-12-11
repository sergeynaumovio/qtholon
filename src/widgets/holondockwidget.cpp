// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holondockwidget.h"
#include "holonabstracttask.h"
#include "holonabstracttaskwindow.h"
#include "holontitlebar.h"
#include "holonwindowarea.h"
#include "holonwindowarea_p.h"
#include <QMainWindow>

class HolonDockWidgetPrivate
{
public:
    HolonDockWidget *const q_ptr;
    HolonAbstractWindow *const window;
    HolonWindowAreaPrivate *const windowarea_d_ptr;
    HolonTitleBar *const titleBar;
    Qt::Orientation orientation{};

    HolonDockWidgetPrivate(HolonDockWidget *q,
                           HolonDesktop *desktop,
                           QMainWindow *parent,
                           HolonAbstractWindow *w = nullptr,
                           HolonWindowAreaPrivate *d = nullptr)
    :   q_ptr(q),
        window(w),
        windowarea_d_ptr(d),
        titleBar(new HolonTitleBar(desktop, q, w, d))
    {
        q_ptr->setTitleBarWidget(titleBar);
        q_ptr->setFeatures(QDockWidget::NoDockWidgetFeatures);
        parent->addDockWidget(Qt::LeftDockWidgetArea, q_ptr);
    }
};

void HolonDockWidget::resizeEvent(QResizeEvent *)
{
    if (isVisible() && d_ptr->windowarea_d_ptr->dockList.size() > 1)
        d_ptr->windowarea_d_ptr->saveWindowAreaState();
}

HolonDockWidget::HolonDockWidget(HolonDesktop *desktop,
                                 QMainWindow *parent,
                                 HolonAbstractWindow *window,
                                 HolonWindowAreaPrivate *d)
:   QDockWidget(parent),
    d_ptr(this, desktop, parent, window, d)
{
    setObjectName(std::as_const(window)->section().last());
    setWidget(window->widget());

    if (HolonAbstractTaskWindow *taskWindow = qobject_cast<HolonAbstractTaskWindow *>(window))
        if (HolonAbstractTask *task = taskWindow->task())
            if (task->isCurrent() && window->isCurrent())
                widget()->setFocus();
}

HolonDockWidget::~HolonDockWidget()
{ }

Qt::Orientation HolonDockWidget::orientation() const
{
    return d_ptr->orientation;
}

void HolonDockWidget::setArea(Qt::DockWidgetArea area)
{
    titleBar()->setDockWidgetArea(area);
}

void HolonDockWidget::setOrientation(Qt::Orientation orientation)
{
    d_ptr->orientation = orientation;
}

HolonTitleBar *HolonDockWidget::titleBar() const
{
    return d_ptr->titleBar;
}

HolonAbstractWindow *HolonDockWidget::window() const
{
    return d_ptr->window;
}
