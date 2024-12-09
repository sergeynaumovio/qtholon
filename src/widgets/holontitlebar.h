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
class QComboBox;

class HolonTitleBar : public QWidget
{
    Q_OBJECT

    const QScopedStorage<HolonTitleBarPrivate, 48> d_ptr;

    QList<HolonAbstractWindow *> siblingWindows(HolonAbstractWindow *window);

protected:
    void paintEvent(QPaintEvent *) override;

public:
    HolonTitleBar(HolonDesktop *desktop,
                  HolonDockWidget *parent,
                  HolonAbstractWindow *window = nullptr,
                  HolonWindowAreaPrivate *d = nullptr);

    ~HolonTitleBar();

    void addWindow(HolonAbstractWindow *window);
    void hideControlButtons();
    void hideSidebarButton();
    void hideSplitButton();
    void setDockWidgetArea(Qt::DockWidgetArea area);
    void showControlButtons();
    void showSidebarButton();
    void showSplitButton();
    QComboBox *windowComboBox() const;
};

#endif // HOLONTITLEBAR_H
