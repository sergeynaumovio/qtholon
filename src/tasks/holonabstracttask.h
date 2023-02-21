// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTTASK_H
#define HOLONABSTRACTTASK_H

#include "qtholonglobal.h"
#include <QLoaderSettings>

class HolonAbstractTaskPrivate;
class HolonTaskModelBranch;
class HolonWorkflowModelBranch;
class HolonAbstractWidget;
class HolonAbstractWindow;

class Q_HOLON_EXPORT HolonAbstractTask : public QObject, public QLoaderSettings
{
    Q_OBJECT

    const QScopedPointer<HolonAbstractTaskPrivate> d_ptr;

protected:
    HolonAbstractTask(QLoaderSettings *settings, HolonTaskModelBranch *taskModelBranch);
    HolonAbstractTask(QLoaderSettings *settings, HolonWorkflowModelBranch *workflowModelBranch);

public:
    ~HolonAbstractTask();

    void addWidget(HolonAbstractWidget *widget);
    void addWindow(HolonAbstractWindow *window);
    virtual int exec() = 0;
    QString group() const;
    bool isCopyable(const QStringList &to) const override;
    virtual QString title() const;
    virtual QWidget *widget(const QString &group) const;
    QList<HolonAbstractWindow *> windows(const QString &group) const;
};

#endif // HOLONABSTRACTTASK_H
