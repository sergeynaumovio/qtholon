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

#include "holonsidebar.h"
#include "holonsidebararea.h"
#include "holonmain.h"

HolonSidebar::HolonSidebar(QLoaderSettings *settings, HolonSidebarArea *parent)
:   HolonTiled(settings, parent)
{
    if (section().last().size() != 1)
    {
        setObjectError("Sidebar name is not a char");
        return;
    }

    QChar sidebarName = section().last().at(0);

    if (!mainWindow()->addSidebarButton(sidebarName))
    {
        setObjectError("Sidebar name is not in list or already used");
        return;
    }

    parent->addWidget(this);

    if (parent->count() == 1 && !parent->isHidden())
        emit mainWindow()->sidebarActivated(section().last());
}
