// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTTASK_H
#define HOLONABSTRACTTASK_H

#include "qtholonglobal.h"
#include "holonnamespace.h"
#include <QLoaderSettings>

class HolonAbstractTaskPrivate;
class HolonAbstractWidget;
class HolonAbstractWindow;
class HolonDesktop;
class HolonTaskFolder;
class HolonWorkflowModelBranch;

class Q_HOLON_EXPORT HolonAbstractTask : public QObject, public QLoaderSettings
{
    Q_OBJECT

    friend class HolonAbstractTaskPrivate;
    friend class HolonDesktopPrivate;
    const QScopedPointer<HolonAbstractTaskPrivate> d_ptr;

protected:
    HolonAbstractTask(QLoaderSettings *settings, HolonTaskFolder *folder);
    HolonAbstractTask(QLoaderSettings *settings, HolonWorkflowModelBranch *workflowModelBranch);

public:
    ~HolonAbstractTask();

    void addWidget(HolonAbstractWidget *widget);
    void addWindow(HolonAbstractWindow *window);
    HolonDesktop *desktop() const;
    virtual int exec() = 0;
    virtual QIcon icon() const;
    bool isCopyable(const QStringList &to) const override;
    bool isCurrent() const;
    virtual int role() const;
    virtual QString title() const;
    virtual QWidget *widget(int role = Holon::NoRole) const;
    QList<HolonAbstractWindow *> windows(int role = Holon::NoRole) const;
};

#endif // HOLONABSTRACTTASK_H
