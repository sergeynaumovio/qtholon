// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
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
    friend class HolonPythonTask;
    const QScopedPointer<HolonAbstractTaskPrivate> d_ptr;

protected:
    HolonAbstractTask(QLoaderSettings *settings, HolonTaskFolder *folder);
    HolonAbstractTask(QLoaderSettings *settings, HolonWorkflow *workflow);

    Q_INVOKABLE bool setValue(const QString &key, const QVariant &value);

public:
    ~HolonAbstractTask();

    void addWindow(HolonAbstractWindow *window);
    virtual QWidget *customWidget(QMetaType taskAttributesWindow);
    HolonDesktop *desktop() const;
    virtual bool exec() = 0;
    virtual QIcon icon() const;
    bool isCopyable(QStringView to) const override;
    bool isCurrent() const;
    bool isOpen() const;
    virtual QWidget *parametersWidget() = 0;
    Q_INVOKABLE virtual QString title() const;
    Q_INVOKABLE QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
};

#endif // HOLONABSTRACTTASK_H
