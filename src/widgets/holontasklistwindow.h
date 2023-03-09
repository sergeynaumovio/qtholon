// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKLISTWINDOW_H
#define HOLONTASKLISTWINDOW_H

#include "holonabstractwindow.h"
#include <QWidget>

class HolonDesktop;
class HolonTaskListWindowPrivate;
class HolonSidebar;

class HolonTaskListWindow : public HolonAbstractWindow
{
    Q_OBJECT

    const QScopedPointer<HolonTaskListWindowPrivate> d_ptr;

public:
    HolonTaskListWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonTaskListWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonTaskListWindow();

    HolonAbstractWindow::Areas areas() const override;
    HolonAbstractWindow::Attributes attributes() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget(const QString &group = QString()) const override;
};

#endif // HOLONTASKLISTWINDOW_H
