// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWMODELBRANCH_H
#define HOLONWORKFLOWMODELBRANCH_H

#include "holonnamespace.h"
#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QObject>

class HolonAbstractTask;
class HolonWorkflowModel;
class HolonWorkflowModelBranchPrivate;

class Q_HOLON_EXPORT HolonWorkflowModelBranch : public QObject, public QLoaderSettings
{
    Q_OBJECT

    friend class HolonWorkflowModelPrivate;
    friend class HolonWorkflowModelBranchPrivate;
    const QScopedPointer<HolonWorkflowModelBranchPrivate> d_ptr;

public:
    explicit HolonWorkflowModelBranch(QLoaderSettings *settings, HolonWorkflowModel *workflowModel);
    ~HolonWorkflowModelBranch();

    void addTask(HolonAbstractTask *task);
    virtual QIcon icon(Holon::TreeItem expanded) const;
    bool isCurrent() const;
    bool isExpanded() const;
    void setCurrent(bool current);
    void setExpanded(bool expanded);
    virtual QString title() const;
    HolonWorkflowModel *workflowModel() const;
};

#endif // HOLONWORKFLOWMODELBRANCH_H
