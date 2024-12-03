// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSIDEBAR_H
#define HOLONSIDEBAR_H

#include "holonwindowarea.h"
#include "qtholonglobal.h"

class HolonDesktop;
class HolonSidebarPrivate;
class QDockWidget;

class Q_HOLON_EXPORT HolonSidebar : public HolonWindowArea
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonSidebar)

    friend class HolonDesktopPrivate;

public:
    explicit HolonSidebar(QLoaderSettings *settings, HolonDesktop *desktop);
    ~HolonSidebar();

    HolonDesktop *desktop() const;
    QString group() const;
    int mainWindowNestingIndex() const;
    QByteArray mainWindowState() const;
    Qt::Orientation orientation() const;
};

#endif // HOLONSIDEBAR_H
