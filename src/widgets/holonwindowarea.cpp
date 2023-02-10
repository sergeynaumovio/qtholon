// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindowarea.h"
#include "holonwindowarea_p.h"
#include <QBoxLayout>
#include <QMainWindow>
#include <QStackedWidget>

HolonWindowArea::HolonWindowArea(HolonWindowAreaPrivate &d,
                                 QLoaderSettings *settings)
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

HolonWindowArea::HolonWindowArea(HolonDesktop *desktop,
                                 QLoaderSettings *settings,
                                 QStackedWidget *parent)
:   HolonWindowArea(*new HolonWindowAreaPrivate(desktop, this), settings)
{
    parent->addWidget(this);
}

HolonWindowArea::~HolonWindowArea()
{ }

void HolonWindowArea::addWindow(HolonWindow *window)
{
    d_ptr->addWindow(window);
}

void HolonWindowArea::closeWindow(HolonWindow *window)
{
    d_ptr->closeWindow(window);
}
