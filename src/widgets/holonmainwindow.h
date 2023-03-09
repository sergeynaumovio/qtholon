// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONMAINWINDOW_H
#define HOLONMAINWINDOW_H

#include <QMainWindow>
#include <QScopedStorage>

class HolonAbstractTask;
class HolonDesktopPrivate;
class HolonMainWindowPrivate;
class HolonSidebar;
class HolonSidebarDock;
class HolonWindowArea;

class HolonMainWindow : public QMainWindow
{
    Q_OBJECT

    const QScopedStorage<HolonMainWindowPrivate, 64> d_ptr;

public:
    HolonMainWindow(HolonDesktopPrivate &desktop_d, QWidget *parent);
    ~HolonMainWindow();

    HolonSidebarDock *addSidebar(HolonSidebar *sidebar);
    HolonWindowArea *addWindowArea(HolonAbstractTask *task);
    void addWindowArea(HolonWindowArea *windowArea);
    void setCurrentTask(HolonAbstractTask *task);
    void setCurrentWindowArea(HolonWindowArea *windowArea);
    HolonWindowArea *windowArea(HolonAbstractTask *task);
};

#endif // HOLONMAINWINDOW_H
