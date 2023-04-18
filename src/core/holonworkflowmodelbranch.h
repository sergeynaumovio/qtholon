// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWMODELBRANCH_H
#define HOLONWORKFLOWMODELBRANCH_H

#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QObject>

class HolonWorkflowModel;

class Q_HOLON_EXPORT HolonWorkflowModelBranch : public QObject, public QLoaderSettings
{
    Q_OBJECT

public:
    explicit HolonWorkflowModelBranch(QLoaderSettings *settings, HolonWorkflowModel *workflowModel);

    virtual QIcon icon() const;
    bool isCurrent() const;
    void setCurrent(bool current);
    virtual QString title() const;
    HolonWorkflowModel *workflowModel() const;
};

#endif // HOLONWORKFLOWMODELBRANCH_H
