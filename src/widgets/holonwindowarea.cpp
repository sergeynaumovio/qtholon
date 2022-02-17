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

#include "holonwindowarea.h"
#include "holonwindowarea_p.h"
#include <QStackedWidget>

HolonWindowArea::HolonWindowArea(HolonWindowAreaPrivate &dd,
                                 QLoaderSettings *settings,
                                 QStackedWidget *parent)
:   QLoaderSettings(settings),
    d_ptr(&dd)
{
    setParent(parent);
}

HolonWindowArea::HolonWindowArea(QLoaderSettings *settings, QStackedWidget *parent)
:   QLoaderSettings(settings),
    d_ptr(new HolonWindowAreaPrivate(this))
{
    setParent(parent);
}

HolonWindowArea::~HolonWindowArea()
{ }

void HolonWindowArea::addWindow(HolonWindow */*window*/)
{

}
