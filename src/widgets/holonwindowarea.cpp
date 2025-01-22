// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindowarea.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonwindowarea_p.h"
#include <QBoxLayout>
#include <QLoaderTree>
#include <QMainWindow>
#include <QStackedWidget>

using namespace Qt::Literals::StringLiterals;

HolonWindowArea::HolonWindowArea(HolonWindowAreaPrivate &d, QLoaderSettings *settings)
:   QLoaderSettings(this, settings),
    d_ptr(&d)
{
    setLayout(new QHBoxLayout(this));
    {
        layout()->addWidget(d_ptr->mainWindow);
        layout()->setContentsMargins({});
    }
}

HolonWindowArea::HolonWindowArea(QLoaderSettings *settings, HolonDesktop *desktop)
:   HolonWindowArea(*new HolonWindowAreaPrivate(this, desktop), settings)
{
    d_ptr->mainWindow->setParent(this);

    if (QObject *object = settings->tree()->object(settings->section()))
        if (qobject_cast<HolonAbstractTask *>(object))
            return;

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

HolonDesktop *HolonWindowArea::desktop() const
{
    return d_ptr->desktop;
}

bool HolonWindowArea::isChecked() const
{
    return value(u"checked"_s, false).toBool();
}

QIcon HolonWindowArea::icon() const
{
    return QIcon(value(u"icon"_s, u":/holon/holoniconlight.svg"_s).toString());
}

QString HolonWindowArea::shortcut() const
{
    return value(u"shortcut"_s).toString();
}

void HolonWindowArea::splitWindow(HolonAbstractWindow *first, HolonAbstractWindow *second, Qt::Orientation orientation)
{
    d_ptr->splitWindow(first, second, orientation);
}

QString HolonWindowArea::title() const
{
     return value(u"title"_s, section()).toString();
}
