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

#include "holonhbox.h"
#include "holondesktop.h"

#include <QHBoxLayout>

HolonHBox::HolonHBox(QLoaderSettings *settings, HolonDesktop *parent)
:   QWidget(parent),
    QLoaderSettings(settings)
{
    if (!parent)
    {
        emitError("HolonDesktop not found");
        return;
    }

    setLayout(new Layout(this));
    {
        layout()->setContentsMargins({});
    }

    parent->setWorkspace(this);
}

HolonDesktop *HolonHBox::desktop() const
{
    return qobject_cast<HolonDesktop*>(parent());
}

HolonHBox::Layout *HolonHBox::layout() const
{
    return static_cast<Layout*>(QWidget::layout());
}

