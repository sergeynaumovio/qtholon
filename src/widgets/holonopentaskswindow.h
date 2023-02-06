// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONOPENTASKSWINDOW_H
#define HOLONOPENTASKSWINDOW_H

#include "holonwindow.h"
#include <QLoaderCopyableInterface>
#include <QWidget>

class HolonDesktop;
class HolonOpenTasksWindowPrivate;
class HolonSidebar;

class HolonOpenTasksWindow : public HolonWindow, public QLoaderCopyableInterface
{
    Q_OBJECT
    Q_INTERFACES(QLoaderCopyableInterface)

    QScopedPointer<HolonOpenTasksWindowPrivate> d_ptr;

public:
    HolonOpenTasksWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonOpenTasksWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonOpenTasksWindow();

    HolonWindow::Areas areas() const override;
    HolonWindow::Attributes attributes() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget() const override;
};

#endif // HOLONOPENTASKSWINDOW_H
