// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONABSTRACTTASK_H
#define HOLONABSTRACTTASK_H

#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QObject>

class HolonAbstractTaskPrivate;
class HolonAbstractWindow;
class HolonDesktop;
class HolonTaskFolder;
class HolonWorkflow;

class Q_HOLON_EXPORT HolonAbstractTask : public QObject, public QLoaderSettings
{
    Q_OBJECT

    friend class HolonAbstractTaskPrivate;
    friend class HolonDesktopPrivate;
    const QScopedPointer<HolonAbstractTaskPrivate> d_ptr;

protected:
    HolonAbstractTask(QLoaderSettings *settings, HolonTaskFolder *folder);
    HolonAbstractTask(QLoaderSettings *settings, HolonWorkflow *workflow);

public:
    ~HolonAbstractTask();

    void addWindow(HolonAbstractWindow *window);
    virtual QWidget *customWidget(QMetaType taskAttributesWindow);
    HolonDesktop *desktop() const;
    virtual int exec() = 0;
    virtual QIcon icon() const;
    bool isCopyable(const QStringList &to) const override;
    bool isCurrent() const;
    bool isOpen() const;
    virtual QWidget *parametersWidget() = 0;
    virtual QString title() const;
};

#endif // HOLONABSTRACTTASK_H
