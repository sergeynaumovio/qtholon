// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstracttaskwindow.h"
#include "holonabstracttask.h"
#include "holonabstracttaskwindow_p.h"
#include "holondesktop.h"
#include "holontaskstackedwindow.h"
#include <QLoaderTree>

using namespace Qt::Literals::StringLiterals;

static HolonAbstractTask *task(HolonTaskStackedWindow *parent)
{
    return qobject_cast<HolonAbstractTask *>(parent->parent());
}

HolonAbstractTaskWindow::HolonAbstractTaskWindow(HolonAbstractTaskWindowPrivate &d,
                                                 QLoaderSettings *settings,
                                                 QObject *parent)
:   HolonAbstractWindow(d, settings, parent)
{ }

HolonAbstractTaskWindow::HolonAbstractTaskWindow(QLoaderSettings *settings, HolonAbstractTask *parent)
:   HolonAbstractWindow(*new HolonAbstractTaskWindowPrivate(this, parent->desktop(), parent), settings, parent)
{
    if (!value(u"current"_s).toBool())
        setValue(u"current"_s, {});
}

HolonAbstractTaskWindow::HolonAbstractTaskWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(*new HolonAbstractTaskWindowPrivate(this, parent, nullptr), settings, parent)
{ }

HolonAbstractTaskWindow::HolonAbstractTaskWindow(QLoaderSettings *settings, HolonTaskStackedWindow *parent)
:   HolonAbstractWindow(*new HolonAbstractTaskWindowPrivate(this, parent->desktop(), ::task(parent)), settings, parent)
{
    if (!value(u"current"_s).toBool())
        setValue(u"current"_s, {});

    if (bool ok = (objectName().toUInt(&ok), ok))
    {
        if (tree()->isLoaded())
            setValue(u"current"_s, true);

        if (!contains(u"open"_s))
            setValue(u"open"_s, true);
    }
    else
        emitError(u"task id not valid"_s);
}

HolonAbstractTaskWindow::~HolonAbstractTaskWindow()
{ }

QWidget *HolonAbstractTaskWindow::customWidget(QMetaType /*taskWindowAttributesWindow*/)
{
    return {};
}

HolonAbstractTask *HolonAbstractTaskWindow::task() const
{
    return d_func()->task;
}
