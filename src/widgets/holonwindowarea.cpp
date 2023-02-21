// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindowarea.h"
#include "holondesktop.h"
#include "holonwindowarea_p.h"
#include <QBoxLayout>
#include <QMainWindow>
#include <QStackedWidget>

HolonWindowArea::HolonWindowArea(HolonWindowAreaPrivate &d, QLoaderSettings *settings)
:   QLoaderSettings(settings),
    d_ptr(&d)
{
    d_ptr->mainWindow->setParent(this);
    setLayout(new QHBoxLayout(this));
    {
        layout()->addWidget(d_ptr->mainWindow);
        layout()->setContentsMargins({});
    }
}

HolonWindowArea::HolonWindowArea(QLoaderSettings *settings, HolonDesktop *desktop)
:   HolonWindowArea(*new HolonWindowAreaPrivate(desktop, this), settings)
{
    desktop->addWindowArea(this);
}

HolonWindowArea::~HolonWindowArea()
{ }

void HolonWindowArea::addWindow(HolonAbstractWindow *window)
{
    d_ptr->addWindow(window);
}

void HolonWindowArea::closeWindow(HolonAbstractWindow *window)
{
    d_ptr->closeWindow(window);
}

bool HolonWindowArea::isChecked() const
{
    return value("checked", false).toBool();
}

QIcon HolonWindowArea::icon() const
{
    return QIcon(value("icon", ":/holon/holoniconlight.svg").toString());
}

QString HolonWindowArea::shortcut() const
{
    return value("shortcut").toString();
}

QString HolonWindowArea::title() const
{
    return value("title", section().constLast()).toString();
}
