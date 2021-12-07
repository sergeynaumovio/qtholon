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

#include "holonmain.h"
#include "holonmain_p.h"
#include "holonstatusbar.h"
#include "holonwidgetinterface.h"

void HolonMain::closeEvent(QCloseEvent*)
{
    hide();
    deleteLater();
}

HolonMain::HolonMain(QLoaderSettings *settings, QWidget *parent)
:   QLoaderSettings(settings),
    d_ptr(new HolonMainPrivate(this))
{
    setParent(parent);

    if (!parent)
        show();

    setStatusBar(new HolonStatusBar(this));
}

HolonMain::~HolonMain()
{ }

void HolonMain::addWidget(HolonWidgetInterface *widget)
{
    QString title = widget->property("title").toString();
    d_ptr->widgets[title] = widget;
}
