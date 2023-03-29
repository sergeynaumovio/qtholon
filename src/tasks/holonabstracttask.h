// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTTASK_H
#define HOLONABSTRACTTASK_H

#include "qtholonglobal.h"
#include <QLoaderSettings>

class HolonAbstractTaskPrivate;
class HolonAbstractWidget;
class HolonAbstractWindow;
class HolonDesktop;
class HolonTaskModelBranch;
class HolonWorkflowModelBranch;

class Q_HOLON_EXPORT HolonAbstractTask : public QObject, public QLoaderSettings
{
    Q_OBJECT

    friend class HolonAbstractTaskPrivate;
    friend class HolonDesktopPrivate;
    const QScopedPointer<HolonAbstractTaskPrivate> d_ptr;

protected:
    HolonAbstractTask(QLoaderSettings *settings, HolonTaskModelBranch *taskModelBranch);
    HolonAbstractTask(QLoaderSettings *settings, HolonWorkflowModelBranch *workflowModelBranch);

public:
    ~HolonAbstractTask();

    void addWidget(HolonAbstractWidget *widget);
    void addWindow(HolonAbstractWindow *window);
    HolonDesktop *desktop() const;
    virtual int exec() = 0;
    QString group() const;
    virtual QIcon icon() const;
    bool isCopyable(const QStringList &to) const override;
    bool isCurrent() const;
    virtual QString title() const;
    virtual QWidget *widget(const QString &group) const;
    QList<HolonAbstractWindow *> windows(const QString &group) const;
};

#endif // HOLONABSTRACTTASK_H
