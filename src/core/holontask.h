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
class HolonWorkspace;

class Q_HOLON_EXPORT HolonTask : public QObject, public QLoaderSettings
{
    Q_OBJECT

    QWidget *widget_ptr{};
    HolonWorkspace *workspace_ptr{};

protected:
    HolonTask(QLoaderSettings *settings, HolonCore *parent);
    HolonTask(QLoaderSettings *settings, HolonDesktop *parent);

    HolonDesktop *desktop() const;

public:
    virtual int exec();
    void setWidget(QWidget *widget);
    void setWorkspace(HolonWorkspace *workspace);
    QWidget *widget() const;
    HolonWorkspace *workspace() const;
};

#endif // HOLONTASK_H
