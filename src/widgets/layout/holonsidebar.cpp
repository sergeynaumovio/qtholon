/****************************************************************************
**
** Copyright (C) 2021, 2022 Sergey Naumov
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
#include "holonmain_p.h"

HolonSidebar::HolonSidebar(QLoaderSettings *settings, HolonSidebarArea *parent)
:   HolonTiled(settings, parent)
{
    if (!mainWindow())
    {
        setObjectError("HolonMain not found");
        return;
    }

    if (section().last().size() != 1)
    {
        setObjectError("sidebar name is not a char");
        return;
    }

    Qt::CheckState checkState = (parent->stateIndex() == parent->count() ? Qt::Checked : Qt::Unchecked);

    if (!mainWindow()->d_ptr->mapSidebar({section().last().at(0), this},
                                         {parent->objectName(), parent}, checkState))
    {
        setObjectError("sidebar name is not in list or already used");
        return;
    }

    parent->addWidget(this);

    if (checkState)
        parent->setCurrentWidget(this);
}
