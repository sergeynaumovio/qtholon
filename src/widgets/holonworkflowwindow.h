// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWWINDOW_H
#define HOLONWORKFLOWWINDOW_H

#include "holonabstractwindow.h"

class HolonDesktop;
class HolonWindowArea;
class HolonWorkflowWindowPrivate;

class HolonWorkflowWindow : public HolonAbstractWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonWorkflowWindow)

public:
    HolonWorkflowWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonWorkflowWindow(QLoaderSettings *settings, HolonWindowArea *parent);
    ~HolonWorkflowWindow();

    QWidget *centralWidget() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbarWidget() const override;
};

#endif // HOLONWORKFLOWWINDOW_H
