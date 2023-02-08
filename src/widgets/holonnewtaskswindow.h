// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONNEWTASKSWINDOW_H
#define HOLONNEWTASKSWINDOW_H

#include "holonwindow.h"
#include <QWidget>

class HolonDesktop;
class HolonNewTasksWindowPrivate;
class HolonSidebar;

class HolonNewTasksWindow : public HolonWindow
{
    Q_OBJECT

    const QScopedPointer<HolonNewTasksWindowPrivate> d_ptr;

public:
    HolonNewTasksWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonNewTasksWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonNewTasksWindow();

    HolonWindow::Areas areas() const override;
    HolonWindow::Attributes attributes() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget() const override;
};

#endif // HOLONNEWTASKSWINDOW_H
