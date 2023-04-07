// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSETTINGSWINDOW_H
#define HOLONSETTINGSWINDOW_H

#include "holonabstractwindow.h"
#include <QScopedStorage>

class HolonDesktop;
class HolonSettingsWindowPrivate;
class HolonSidebar;

class HolonSettingsWindow : public HolonAbstractWindow
{
    Q_OBJECT

    const QScopedStorage<HolonSettingsWindowPrivate, 40> d_ptr;

public:
    HolonSettingsWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonSettingsWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonSettingsWindow();

    Holon::WindowFlags flags() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    int role() const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget(int role = Holon::NoRole) const override;
};

#endif // HOLONSETTINGSWINDOW_H
