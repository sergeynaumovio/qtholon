// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsettingswindow.h"
#include "holondesktop.h"
#include "holonid.h"
#include "holonsidebar.h"
#include <QLoaderTree>

using namespace Qt::Literals::StringLiterals;

HolonSettingsWindow::HolonSettingsWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonTaskWindowAttributesWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonSettingsWindow::HolonSettingsWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonTaskWindowAttributesWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonSettingsWindow::~HolonSettingsWindow()
{ }

QIcon HolonSettingsWindow::icon() const
{
    return {};
}

bool HolonSettingsWindow::isCopyable(QStringView to) const
{
    QStringView toParent = HolonId::parentSection(to);

    if (toParent.size())
    {
        if (QObject *parent = tree()->object(toParent))
            if (qobject_cast<HolonSidebar *>(parent))
                return true;
    }

    return false;
}

QString HolonSettingsWindow::title() const
{
    return u"Settings"_s;
}

QWidget *HolonSettingsWindow::toolbarWidget()
{
    return {};
}
