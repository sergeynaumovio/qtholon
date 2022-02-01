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

#include "holonmenu.h"
#include "holonmenu_p.h"
#include "holontaskbar.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

HolonMenu::HolonMenu(QLoaderSettings *settings, HolonTaskbar *parent)
:   QPushButton(QIcon(":/holon/holoniconlight.svg"), "", parent),
    QLoaderSettings(settings),
    d_ptr(new HolonMenuPrivate(this, parent))
{
    if (!parent)
    {
        emitError("HolonTaskbar not found");
        return;
    } 

    setFlat(true);

    QString stretch = value("stretch").toString();
    if (stretch.contains("before"))
        parent->layout()->addStretch();

    parent->layout()->addWidget(this);

    if (stretch.contains("after"))
        parent->layout()->addStretch();

    int size = parent->preferedHeight();
    if (parent->area() == HolonTaskbar::Top || parent->area() == HolonTaskbar::Bottom)
        setFixedSize(size, size);
    else
        setFixedHeight(size);

    size *= 0.5;
    setIconSize({size, size});
}

HolonMenu::~HolonMenu()
{ }

HolonTaskbar *HolonMenu::taskbar() const
{
    return d_ptr->taskbar;
}
