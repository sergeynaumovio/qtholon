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
#include "holondesktop.h"
#include "holondesktop_p.h"
#include <QLoaderTree>

HolonSidebar::HolonSidebar(QLoaderSettings *settings, HolonSidebarArea *parent)
:   HolonTiled(settings, parent)
{
    if (!desktop())
    {
        emitError("HolonDesktop not found");
        return;
    }

    if (section().last().size() != 1)
    {
        emitError("sidebar name is not a char");
        return;
    }

    Qt::CheckState checkState = (!parent->isHidden() && parent->stateIndex() == parent->count() ? Qt::Checked : Qt::Unchecked);

    /*if (!desktop()->d_ptr->mapSidebar({section().last().at(0), this},
                                 {parent->objectName(), parent}, checkState))
    {
        emitError("sidebar name is not in list or already used");
        return;
    }
*/
    parent->addSidebar(this);

    if (checkState)
        parent->setCurrentWidget(this);
}

bool HolonSidebar::move(const QStringList &to)
{
    HolonSidebarArea *area = qobject_cast<HolonSidebarArea*>(tree()->object(to));
    if (!area)
        return false;

    SidebarRelatedObjects &sidebarObjects = desktop()->d_ptr->sidebarRelatedObjects[objectName().at(0)];
    SidebarButton *button = sidebarObjects.button;
    HolonSidebarArea *prev = sidebarObjects.area;

    if (button->isChecked())
        prev->hide();
    else
        button->setChecked(true);

    if (isVisible())
    {
        for (SidebarRelatedObjects value : qAsConst(desktop()->d_ptr->sidebarRelatedObjects))
        {
            if (value.sidebar == area->currentWidget())
                value.button->setChecked(false);
        }
    }
    else
        show();

    QStackedWidget *widget = area;
    widget->addWidget(this);
    widget->setCurrentWidget(this);

    button->area = area->objectName();
    sidebarObjects.area = area;

    return true;
}
