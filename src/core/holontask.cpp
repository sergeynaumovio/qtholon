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

#include "holontask.h"
#include "holoncore.h"
#include "holondesktop.h"

HolonTask::HolonTask(QLoaderSettings *settings, HolonCore *parent)
:   QObject(parent),
    QLoaderSettings(settings)
{ }

HolonTask::HolonTask(QLoaderSettings *settings, HolonDesktop *parent)
:   QObject(parent),
    QLoaderSettings(settings)
{
    parent->addTask(this);
}

HolonDesktop *HolonTask::desktop() const
{
    return qobject_cast<HolonDesktop*>(parent());
}

int HolonTask::exec()
{
    return 0;
}

void HolonTask::setWidget(QWidget *widget)
{
    widget_ptr = widget;
}

void HolonTask::setWorkspace(HolonWorkspace *workspace)
{
    workspace_ptr = workspace;
}

QWidget *HolonTask::widget() const
{
    return widget_ptr;
}

HolonWorkspace *HolonTask::workspace() const
{
    return workspace_ptr;
}
