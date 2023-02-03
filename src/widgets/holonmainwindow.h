// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONMAINWINDOW_H
#define HOLONMAINWINDOW_H

#include <QMainWindow>

class HolonDesktopPrivate;
class HolonMainWindowPrivate;
class HolonSidebar;
class HolonSidebarDock;

class HolonMainWindow : public QMainWindow
{
    Q_OBJECT

    HolonMainWindowPrivate &d;
    std::aligned_storage_t<48, sizeof (ptrdiff_t)> d_storage;

public:
    HolonMainWindow(HolonDesktopPrivate &desktop_d, QWidget *parent);
    ~HolonMainWindow();

    HolonSidebarDock *addSidebar(HolonSidebar *sidebar);
};

#endif // HOLONMAINWINDOW_H
