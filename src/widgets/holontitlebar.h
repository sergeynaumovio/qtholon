// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTITLEBAR_H
#define HOLONTITLEBAR_H

#include <QWidget>

class HolonAbstractWindow;
class HolonDesktop;
class HolonDockWidget;
class HolonTitleBarPrivate;
class HolonWindowAreaPrivate;

class HolonTitleBar : public QWidget
{
    Q_OBJECT

    HolonTitleBarPrivate &d;
    std::aligned_storage_t<16, sizeof (ptrdiff_t)> d_storage;

protected:
    void paintEvent(QPaintEvent *) override;

public:
    HolonTitleBar(HolonDesktop *desktop,
                  HolonDockWidget *parent,
                  HolonAbstractWindow *window = nullptr,
                  HolonWindowAreaPrivate *d = nullptr);

    void hideControlButtons();
    void showControlButtons();
};

#endif // HOLONTITLEBAR_H
