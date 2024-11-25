// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOW_H
#define HOLONWORKFLOW_H

#include <QLoaderSettings>

class HolonAbstractTask;
class HolonCore;
class HolonDesktop;
class HolonWorkflowPrivate;

class HolonWorkflow : public QObject, public QLoaderSettings
{
    Q_OBJECT

    friend class HolonWorkflowPrivate;
    const QScopedPointer<HolonWorkflowPrivate> d_ptr;

public:
    explicit HolonWorkflow(QLoaderSettings *settings, HolonCore *core);
    explicit HolonWorkflow(QLoaderSettings *settings, HolonDesktop *desktop);

    ~HolonWorkflow();

    void addTask(HolonAbstractTask *task);
    HolonCore *core() const;
    HolonDesktop *desktop() const;
    bool isCurrent() const;
};

#endif // HOLONWORKFLOW_H
