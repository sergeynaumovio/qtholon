/****************************************************************************
**
** Copyright (C) 2022 Sergey Naumov
**
** Permission to use, copy, modify, and/or distribute this
** software for any purpose with or without fee is hereby granted.
**
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
** WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL
** THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
** CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
** LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
** NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
** CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
**
****************************************************************************/

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
