// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKLISTWINDOW_H
#define HOLONTASKLISTWINDOW_H

#include "holonabstractwindow.h"
#include <QScopedStorage>

class HolonDesktop;
class HolonTaskListWindowPrivate;
class HolonSidebar;

class HolonTaskListWindow : public HolonAbstractWindow
{
    Q_OBJECT

    const QScopedStorage<HolonTaskListWindowPrivate, 40> d_ptr;

public:
    HolonTaskListWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonTaskListWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonTaskListWindow();

    Holon::WindowFlags flags() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget(const QString &group = QString()) const override;
};

#endif // HOLONTASKLISTWINDOW_H
