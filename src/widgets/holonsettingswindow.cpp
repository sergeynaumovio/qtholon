// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsettingswindow.h"
#include "holondesktop.h"
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

bool HolonSettingsWindow::isCopyable(const QStringList &to) const
{
    QStringList parentSection = to;
    if (to.size() > 1)
    {
        parentSection.removeLast();
        QObject *parent = tree()->object(parentSection);
        if (qobject_cast<HolonSidebar *>(parent))
            return true;
    }

    return false;
}

int HolonSettingsWindow::role() const
{
    return Holon::SettingsRole;
}

QString HolonSettingsWindow::title() const
{
    return u"Settings"_s;
}

QWidget *HolonSettingsWindow::toolbarWidget() const
{
    return {};
}
