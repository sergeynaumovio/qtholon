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

    friend class HolonWorkflowWindowPrivate;

public:
    HolonWorkflowWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonWorkflowWindow(QLoaderSettings *settings, HolonWindowArea *parent);
    ~HolonWorkflowWindow();

    QWidget *centralWidget() override;
    int current() const;
    QIcon icon() const override;
    bool isCopyable(QStringView to) const override;
    bool isTreeView() const;
    void setCurrent(int id);
    void setTreeView(bool value);
    QString title() const override;
    QWidget *toolbarWidget() override;
};

#endif // HOLONWORKFLOWWINDOW_H
