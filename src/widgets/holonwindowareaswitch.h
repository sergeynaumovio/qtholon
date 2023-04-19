// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWINDOWAREASWITCH_H
#define HOLONWINDOWAREASWITCH_H

#include <QScopedStorage>
#include <QStyle>
#include <QToolButton>
#include <QWidget>

class HolonDesktopPrivate;
class HolonSidebar;
class HolonTaskbar;
class HolonWindowArea;
class HolonWindowAreaSwitchPrivate;

class HolonWindowAreaSwitch : public QWidget
{
    Q_OBJECT

    const QScopedStorage<HolonWindowAreaSwitchPrivate, 32> d_ptr;

public:
    HolonWindowAreaSwitch(HolonDesktopPrivate &desktop_d, HolonTaskbar *parent);
    ~HolonWindowAreaSwitch();

    void addSidebar(HolonSidebar *sidebar);
    void addWindowArea(HolonWindowArea *windowArea);
    void closeWindowArea(HolonWindowArea *windowArea);
};

class HolonSwitchButton : public QToolButton
{
    Q_OBJECT

    const QStyle::PrimitiveElement pe;

protected:
    HolonWindowAreaSwitchPrivate &switch_d;
    HolonDesktopPrivate &desktop_d;

    void paintEvent(QPaintEvent *) override;

    HolonSwitchButton(HolonWindowAreaSwitchPrivate &swtch_d,
                      HolonWindowArea *windowArea,
                      HolonWindowAreaSwitch *parent);
};

class HolonSidebarButton : public HolonSwitchButton
{
    Q_OBJECT

public:
    HolonSidebarButton(HolonWindowAreaSwitchPrivate &swtch_d,
                       HolonSidebar *sidebar,
                       HolonWindowAreaSwitch *parent);
};

class HolonWindowAreaButton : public HolonSwitchButton
{
    Q_OBJECT

public:
    HolonWindowAreaButton(HolonWindowAreaSwitchPrivate &swtch_d,
                          HolonWindowArea *windowArea,
                          HolonWindowAreaSwitch *parent);
};

#endif // HOLONWINDOWAREASWITCH_H
