// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWSWINDOW_H
#define HOLONWORKFLOWSWINDOW_H

#include "holonwindow.h"
#include <QWidget>

class HolonDesktop;
class HolonWindowArea;
class HolonWorkflowsWindowPrivate;

class HolonWorkflowsWindow : public HolonWindow
{
    Q_OBJECT

    const QScopedPointer<HolonWorkflowsWindowPrivate> d_ptr;

public:
    HolonWorkflowsWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonWorkflowsWindow(QLoaderSettings *settings, HolonWindowArea *parent);
    ~HolonWorkflowsWindow();

    HolonWindow::Areas areas() const override;
    HolonWindow::Attributes attributes() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget() const override;
};

#endif // HOLONWORKFLOWSWINDOW_H
