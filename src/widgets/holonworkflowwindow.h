// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWWINDOW_H
#define HOLONWORKFLOWWINDOW_H

#include "holonabstractwindow.h"
#include "holonnamespace.h"
#include <QScopedStorage>

class HolonDesktop;
class HolonWindowArea;
class HolonWorkflowWindowPrivate;

class HolonWorkflowWindow : public HolonAbstractWindow
{
    Q_OBJECT

    const QScopedStorage<HolonWorkflowWindowPrivate, 24> d_ptr;

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
