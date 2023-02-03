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
    HolonSidebar(QLoaderSettings *settings, HolonDesktop *desktop);
    ~HolonSidebar();

    bool isChecked() const;
    virtual QIcon icon() const;
    virtual QString title() const;
};

#endif // HOLONSIDEBAR_H
