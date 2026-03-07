// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWWINDOW_H
#define HOLONWORKFLOWWINDOW_H

#include "holonabstractwindow.h"
#include "holonsettingswidgetinterface.h"

class HolonDesktop;
class HolonWindowArea;
class HolonWorkflowWindowPrivate;

class HolonWorkflowWindow : public HolonAbstractWindow,
                            public HolonSettingsWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(HolonSettingsWidgetInterface)
    Q_DECLARE_PRIVATE(HolonWorkflowWindow)

    friend class HolonWorkflowWindowPrivate;

public:
    HolonWorkflowWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonWorkflowWindow(QLoaderSettings *settings, HolonWindowArea *parent);
    ~HolonWorkflowWindow();

    QWidget *centralWidget() override;
    QIcon icon() const override;
    bool isCopyable(QStringView to) const override;
    bool isTreeView() const;
    QWidget *settingsWidget() override;
    void setTreeView(bool value);
    void setWorkflowId(int id);
    QString title() const override;
    QWidget *toolbarWidget() override;
    int workflowId() const;
};

#endif // HOLONWORKFLOWWINDOW_H
