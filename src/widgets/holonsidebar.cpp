// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsidebar.h"
#include "holondesktop.h"
#include "holonsidebar_p.h"
#include "holonwindowarea_p.h"
#include <QBoxLayout>
#include <QIcon>
#include <QLabel>

HolonSidebar::HolonSidebar(QLoaderSettings *settings, HolonDesktop *desktop)
:   HolonWindowArea(*new HolonSidebarPrivate(desktop, this), settings)
{
    desktop->addSidebar(this);
}

HolonSidebar::~HolonSidebar()
{ }

QIcon HolonSidebar::icon() const
{
    return QIcon(value("icon", ":/holon/holoniconlight.svg").toString());
}

QString HolonSidebar::title() const
{
    return value("title", section().constLast()).toString();
}
