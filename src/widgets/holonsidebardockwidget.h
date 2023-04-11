// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSIDEBARDOCKWIDGET_H
#define HOLONSIDEBARDOCKWIDGET_H

#include <QDockWidget>
#include <QScopedStorage>

class HolonDesktop;
class HolonDesktopPrivate;
class HolonSidebar;
class HolonSidebarDockWidgetPrivate;
class HolonSidebarMainWindow;

class HolonSidebarDockWidget : public QDockWidget
{
    Q_OBJECT

    friend class HolonDesktopPrivate;
    const QScopedStorage<HolonSidebarDockWidgetPrivate, 88> d_ptr;

protected:
    void resizeEvent(QResizeEvent *) override;

public:
    HolonSidebarDockWidget(HolonDesktopPrivate &desktop_d,
                           const QString &name,
                           HolonSidebarMainWindow *parent);

    ~HolonSidebarDockWidget();

    void addSidebar(HolonSidebar *sidebar);
    QList<HolonSidebar *> sidebars() const;
    void showTitleBarWidget(bool show);
};

#endif // HOLONSIDEBARDOCKWIDGET_H
