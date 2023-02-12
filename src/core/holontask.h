// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASK_H
#define HOLONTASK_H

#include "qtholonglobal.h"
#include <QLoaderSettings>

class HolonNewTasksDir;
class HolonOpenTasksDir;
class HolonWindow;

class HolonTaskData
{
public:
    QList<HolonWindow*> attributes;
    QList<HolonWindow*> central;
    HolonWindow *control{};
};

class Q_HOLON_EXPORT HolonTask : public QObject, public QLoaderSettings
{
    Q_OBJECT

    const QScopedPointer<HolonTaskData> d_ptr;

protected:
    HolonTask(QLoaderSettings *settings, HolonNewTasksDir *newTasksDir);
    HolonTask(QLoaderSettings *settings, HolonOpenTasksDir *openTasksDir);

public:
    ~HolonTask();

    void addAttributesWindow(HolonWindow *window) { d_ptr->attributes.append(window); }
    void addCentralWindow(HolonWindow *window) { d_ptr->central.append(window); }
    const QList<HolonWindow*> &attributesWindowList() { return d_ptr->attributes; }
    const QList<HolonWindow*> &centralWindowList() { return d_ptr->central; }
    HolonWindow *controlWindow() { return d_ptr->control; }
    virtual int exec() = 0;
    bool isCopyable(const QStringList &to) const override;
    void setControlWindow(HolonWindow *window) { d_ptr->control = window; }
};

#endif // HOLONTASK_H
