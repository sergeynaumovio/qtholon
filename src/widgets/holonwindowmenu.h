// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWINDOWMENU_H
#define HOLONWINDOWMENU_H

#include "qtholonglobal.h"
#include <QWidget>

class HolonDesktop;
class HolonWindow;

class Q_HOLON_EXPORT HolonWindowMenu : public QWidget
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent *) override;

Q_SIGNALS:
    void triggered(HolonWindow *window);

public:
    HolonWindowMenu(HolonDesktop *desktop, QWidget *parent);
};

#endif // HOLONWINDOWMENU_H
