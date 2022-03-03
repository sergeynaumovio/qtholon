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
#include "holonwindowarea_p.h"
#include "holondesktop.h"
#include "holontaskbar.h"
#include "holonwindow.h"
#include <QBoxLayout>
#include <QLabel>

class HolonSidebarPrivate : public HolonWindowAreaPrivate
{
public:
    QChar sidebar;
    QString sidebarArea;

    HolonSidebarPrivate(HolonWindowArea *q, HolonDesktop *desktop)
    :   HolonWindowAreaPrivate(q, desktop)
    { }
};

HolonSidebar::HolonSidebar(QLoaderSettings *settings, HolonDesktop *desktop)
:   HolonWindowArea(*new HolonSidebarPrivate(this, desktop), settings)
{
    Q_D(HolonSidebar);
    d->sidebar = value("sidebar").toChar();
    d->sidebarArea = value("sidebarArea").toString();

    if (!desktop->sidebarList().contains(d->sidebar))
    {
        emitError("sidebar is not in list");
        return;
    }

    if (!desktop->sidebarAreaList().contains(d->sidebarArea))
    {
        emitError("sidebarArea is not in list");
        return;
    }

    if (!desktop->addSidebar(this))
    {
        emitError("sidebar is already added");
        return;
    }
}

HolonSidebar::~HolonSidebar()
{ }

QChar HolonSidebar::sidebar() const
{
    Q_D(const HolonSidebar);
    return d->sidebar;
}

QString HolonSidebar::sidebarArea() const
{
    Q_D(const HolonSidebar);
    return d->sidebarArea;
}

HolonSidebarSwitch::HolonSidebarSwitch(QLoaderSettings *settings, HolonTaskbar *parent)
:   QWidget(parent),
    QLoaderSettings(settings)
{
    if (parent->area() == HolonTaskbar::Top ||
        parent->area() == HolonTaskbar::Bottom)
    {
        setLayout(new QHBoxLayout(this));
    }
    else
    {
        setLayout(new QVBoxLayout(this));
    }

    layout()->addWidget(new QLabel("Taskbar Switch", this));

    parent->addWidget(this);
}
