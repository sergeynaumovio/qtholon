// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontaskfolder.h"
#include "holondesktop.h"
#include "holontheme.h"
#include "holonthemeicons.h"
#include <QIcon>

using namespace Qt::Literals::StringLiterals;

class HolonTaskFolderPrivate
{
public:
    HolonDesktop *const desktop;
    HolonTaskFolder *const parentFolder;
    bool expanded{};

    HolonTaskFolderPrivate(HolonDesktop *d)
    :   desktop(d),
        parentFolder(nullptr)
    { }

    HolonTaskFolderPrivate(HolonTaskFolder *f)
    :   desktop(nullptr),
        parentFolder(f)
    { }
};

HolonTaskFolder::HolonTaskFolder(QLoaderSettings *settings, HolonDesktop *desktop)
:   QObject(desktop),
    QLoaderSettings(this, settings),
    d_ptr(desktop)
{ }

HolonTaskFolder::HolonTaskFolder(QLoaderSettings *settings, HolonTaskFolder *folder)
:   QObject(folder),
    QLoaderSettings(this, settings),
    d_ptr(folder)
{ }

HolonTaskFolder::~HolonTaskFolder()
{ }

HolonDesktop *HolonTaskFolder::desktop() const
{
    if (d_ptr->desktop)
        return d_ptr->desktop;

    return d_ptr->parentFolder->desktop ();
}

QIcon HolonTaskFolder::icon(Holon::TreeItem state) const
{
    if (state == Holon::Collapsed)
        return desktop()->theme()->icons()->dirClosedIcon();

    return desktop()->theme()->icons()->dirOpenIcon();
}

bool HolonTaskFolder::isExpanded() const
{
    return d_ptr->expanded;
}

void HolonTaskFolder::setExpanded(bool expanded)
{
    d_ptr->expanded = expanded;
}

QString HolonTaskFolder::title() const
{
    return value(u"title"_s, section()).toString();
}
