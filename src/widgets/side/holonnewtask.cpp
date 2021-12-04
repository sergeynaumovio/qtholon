/****************************************************************************
**
** Copyright (C) 2021 Sergey Naumov
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

#include "holonnewtask.h"
#include "holonmainwindow.h"
#include "holonsidebar.h"

HolonNewTask::HolonNewTask(QLoaderSettings *settings, HolonMainWindow *parent)
:   HolonSideWidget(settings, parent)
{
    parent->addSideWidget(this);
}

HolonNewTask::HolonNewTask(QLoaderSettings *settings, HolonSideBar *parent)
:   HolonSideWidget(settings, parent)
{
    parent->addSideWidget(this);
}

QWidget *HolonNewTask::widget()
{
    return nullptr;
}
