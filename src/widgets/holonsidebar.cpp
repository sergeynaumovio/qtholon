// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsidebar.h"
#include "holondesktop.h"
#include "holonsidebar_p.h"
#include <QMainWindow>

using namespace Qt::Literals::StringLiterals;

HolonSidebar::HolonSidebar(QLoaderSettings *settings, HolonDesktop *desktop)
:   HolonWindowArea(*new HolonSidebarPrivate(desktop, this), settings)
{
    d_ptr->mainWindow->setParent(this);
    desktop->addSidebar(this);
}

HolonSidebar::~HolonSidebar()
{ }

HolonDesktop *HolonSidebar::desktop() const
{
    return d_ptr->desktop;
}

QString HolonSidebar::group() const
{
    return value(u"group"_s).toString();
}
