// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTITLEBAR_H
#define HOLONTITLEBAR_H

#include <QScopedStorage>
#include <QWidget>

class HolonAbstractWindow;
class HolonDesktop;
class HolonDockWidget;
class HolonTitleBarPrivate;
class HolonWindowAreaPrivate;

class HolonTitleBar : public QWidget
{
    Q_OBJECT

    const QScopedStorage<HolonTitleBarPrivate, 24> d_ptr;

protected:
    void paintEvent(QPaintEvent *) override;

public:
    HolonTitleBar(HolonDesktop *desktop,
                  HolonDockWidget *parent,
                  HolonAbstractWindow *window = nullptr,
                  HolonWindowAreaPrivate *d = nullptr);

    ~HolonTitleBar();

    void hideControlButtons();
    void hideSplitButton();
    void setDockWidgetArea(Qt::DockWidgetArea area);
    void showControlButtons();
    void showSplitButton();
};

#endif // HOLONTITLEBAR_H
