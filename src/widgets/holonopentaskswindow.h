// Copyright (C) 2022 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONOPENTASKSWINDOW_H
#define HOLONOPENTASKSWINDOW_H

#include "holonwindow.h"
#include <QWidget>
#include <QLoaderCopyInterface>

class HolonOpenTasksWindowPrivate;
class HolonDesktop;
class HolonSidebar;

class HolonOpenTasksWindow : public HolonWindow, public QLoaderCopyInterface
{
    Q_OBJECT
    Q_INTERFACES(QLoaderCopyInterface)

    QScopedPointer<HolonOpenTasksWindowPrivate> d_ptr;

public:
    Q_INVOKABLE HolonOpenTasksWindow(QLoaderSettings *settings, HolonDesktop *parent);
    Q_INVOKABLE HolonOpenTasksWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonOpenTasksWindow();

    HolonWindow::Areas areas() const override;
    bool copy(const QStringList &to) const override;
    void copy(QObject *from) override;
    QIcon icon() const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget() const override;

};

class HolonOpenTasksToolbar : public QWidget
{

};

class HolonOpenTasksWidget : public QWidget
{

};

#endif // HOLONOPENTASKSWINDOW_H
