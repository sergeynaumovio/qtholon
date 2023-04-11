// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSIDEBARDOCKWIDGET_P_H
#define HOLONSIDEBARDOCKWIDGET_P_H

#include <QScopedStorage>

class HolonDesktopPrivate;
class HolonSidebar;
class HolonSidebarDockWidget;
class HolonSidebarDockWidgetPrivateData;
class HolonSidebarMainWindow;
template<typename> class QList;
class QString;

class HolonSidebarDockWidgetPrivate
{
    const QScopedStorage<HolonSidebarDockWidgetPrivateData, 88> d_ptr;

public:
    HolonSidebarDockWidgetPrivate(HolonDesktopPrivate &desktop_d,
                                  HolonSidebarDockWidget *q,
                                  const QString &name,
                                  HolonSidebarMainWindow *parent);

    ~HolonSidebarDockWidgetPrivate();

    void addSidebar(HolonSidebar *sidebar);
    HolonSidebar *currentSidebar() const;
    HolonDesktopPrivate &desktop_d() const;
    void setSidebar(HolonSidebar *sidebar);
    const QList<HolonSidebar *> &sidebars() const;
    void showTitleBarWidget(bool show) const;
    HolonSidebarMainWindow *sidebarMainWindow() const;
};

#endif // HOLONSIDEBARDOCKWIDGET_P_H
