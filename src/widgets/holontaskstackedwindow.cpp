// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontaskstackedwindow.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonstackedwidget.h"
#include "holontaskstackedwindow_p.h"
#include <QLoaderTree>

HolonTaskStackedWindow::HolonTaskStackedWindow(QLoaderSettings *settings, HolonAbstractTask *parent)
:   HolonStackedWindow(*new HolonTaskStackedWindowPrivate(this, parent->desktop(), parent), settings, parent)
{
    parent->addWindow(this);
}

HolonTaskStackedWindow::~HolonTaskStackedWindow()
{ }

bool HolonTaskStackedWindow::isCopyable(const QStringList &to) const
{
    QStringList parentSection = to;
    if (to.size() > 1)
    {
        parentSection.removeLast();
        QObject *parent = tree()->object(parentSection);
        if (qobject_cast<HolonAbstractTask *>(parent))
            return true;
    }

    return false;
}

HolonAbstractTask *HolonTaskStackedWindow::task() const
{
    return d_func()->task;
}
