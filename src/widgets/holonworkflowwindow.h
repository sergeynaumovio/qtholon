// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWWINDOW_H
#define HOLONWORKFLOWWINDOW_H

#include "holonabstractwindow.h"
#include <QWidget>

class HolonDesktop;
class HolonWindowArea;
class HolonWorkflowWindowPrivate;

class HolonWorkflowWindow : public HolonAbstractWindow
{
    Q_OBJECT

    const QScopedPointer<HolonWorkflowWindowPrivate> d_ptr;

public:
    HolonWorkflowWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonWorkflowWindow(QLoaderSettings *settings, HolonWindowArea *parent);
    ~HolonWorkflowWindow();

    HolonAbstractWindow::Area area() const override;
    HolonAbstractWindow::Attributes attributes() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget(HolonAbstractWindow::Area area) const override;
};

#endif // HOLONWORKFLOWWINDOW_H
