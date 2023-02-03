// Copyright (C) 2022 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASK_H
#define HOLONTASK_H

#include "qtholonglobal.h"
#include <QLoaderSettings>

class HolonCore;
class HolonDesktop;
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

protected:
    const QScopedPointer<HolonTaskData> d_ptr;

    HolonTask(QLoaderSettings *settings, HolonCore *parent);
    HolonTask(QLoaderSettings *settings, HolonDesktop *parent);
    virtual ~HolonTask();

public:
    void addAttributesWindow(HolonWindow *window) { d_ptr->attributes.append(window); }
    void addCentralWindow(HolonWindow *window) { d_ptr->central.append(window); }
    const QList<HolonWindow*> &attributesWindowList() { return d_ptr->attributes; }
    const QList<HolonWindow*> &centralWindowList() { return d_ptr->central; }
    HolonWindow *controlWindow() { return d_ptr->control; }
    virtual int exec();
    void setControlWindow(HolonWindow *window) { d_ptr->control = window; }
};

#endif // HOLONTASK_H
