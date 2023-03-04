// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKBAR_H
#define HOLONTASKBAR_H

#include "holonalignedstorage.h"
#include <QWidget>

class HolonDesktopPrivate;
class HolonWindowAreaSwitch;
class HolonTaskbarPrivate;
class QBoxLayout;

class HolonTaskbar : public QWidget
{
    Q_OBJECT

    const HolonAlignedStorage<HolonTaskbarPrivate, 16> d_ptr;

    QBoxLayout *layout() const;

protected:
    void paintEvent(QPaintEvent *) override;

public:
    HolonTaskbar(HolonDesktopPrivate &desktop_d);
    ~HolonTaskbar();

    void addWidget(QWidget *widget);
    HolonWindowAreaSwitch *sidebarSwitch() const;
};

#endif // HOLONTASKBAR_H
