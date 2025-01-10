// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOW_H
#define HOLONWORKFLOW_H

#include <QLoaderSettings>
#include <QScopedStorage>

class HolonAbstractTask;
class HolonCore;
class HolonDesktop;
class HolonWorkflowPrivate;

class HolonWorkflow : public QObject, public QLoaderSettings
{
    Q_OBJECT

    friend class HolonWorkflowPrivate;
    const QScopedStorage<HolonWorkflowPrivate, 72> d_ptr;

public:
    HolonWorkflow(QLoaderSettings *settings, HolonCore *core);
    HolonWorkflow(QLoaderSettings *settings, HolonDesktop *desktop);
    HolonWorkflow(QLoaderSettings *settings, HolonWorkflow *workflow);

    ~HolonWorkflow();

    void addTask(HolonAbstractTask *task);
    HolonCore *core() const;
    HolonDesktop *desktop() const;
    bool isCurrent() const;
    QString title() const;
};

#endif // HOLONWORKFLOW_H
