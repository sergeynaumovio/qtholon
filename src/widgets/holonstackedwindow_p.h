// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSTACKEDWINDOW_P_H
#define HOLONSTACKEDWINDOW_P_H

class HolonAbstractWindow;
class HolonDesktop;
class HolonStackedWindow;
class HolonTitleBar;
class HolonWindowStackedWidget;

class HolonStackedWindowPrivate
{
public:
    HolonStackedWindow *const q_ptr;
    HolonDesktop *const desktop;
    HolonWindowStackedWidget *const stackedWidget;
    HolonTitleBar *titleBar;
    HolonAbstractWindow *currentWindow{};

    HolonStackedWindowPrivate(HolonStackedWindow *q, HolonDesktop *desk);

    void addWindow(HolonAbstractWindow *window);
    void setWindow(HolonAbstractWindow *window);
};

#endif // HOLONSTACKEDWINDOW_P_H
