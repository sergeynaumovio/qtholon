// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWINDOWMENU_H
#define HOLONWINDOWMENU_H

#include <QWidget>

class HolonAbstractWindow;
class HolonDesktop;

class HolonWindowMenu : public QWidget
{
    Q_OBJECT

    template<typename T>
    T findParent();

protected:
    void paintEvent(QPaintEvent *) override;

Q_SIGNALS:
    void triggered(HolonAbstractWindow *window);

public:
    HolonWindowMenu(HolonDesktop *desktop, QWidget *parent);
};

#endif // HOLONWINDOWMENU_H
