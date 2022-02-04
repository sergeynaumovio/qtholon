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

#include "holondesktop.h"
#include "holondesktop_p.h"

void HolonDesktop::closeEvent(QCloseEvent *)
{
    hide();
    deleteLater();
}

void HolonDesktop::resizeEvent(QResizeEvent *)
{
    emit sizeChanged(size());
}

QVariant HolonDesktop::fromString(const QString &string) const
{
    return QLoaderSettings::fromString(string);
}

QString HolonDesktop::fromVariant(const QVariant &variant) const
{
    return QLoaderSettings::fromVariant(variant);
}

HolonDesktop::HolonDesktop(QLoaderSettings *settings, QWidget *parent)
:   QLoaderSettings(settings),
    d_ptr(new HolonDesktopPrivate(this))
{
    setParent(parent);

    if (!parent)
        show();

    if (!contains("sidebarAreaList"))
    {
        emitError("sidebarAreaList property is not set");
        return;
    }

    if (!d_ptr->setSidebarAreas(value("sidebarAreaList").toStringList()))
    {
        emitError("sidebarArea name is not set");
        return;
    }

    if (contains("sidebarList"))
    {
        d_ptr->sidebarList = value("sidebarList").value<QCharList>();
        if (!d_ptr->sidebarList.size())
        {
            emitError("sidebarList item is not a char");
            return;
        }
    }
    else
    {
        emitError("sidebarList property is not set");
        return;
    }

    d_ptr->setDesktopLayout();

}

HolonDesktop::~HolonDesktop()
{ }

QStringList HolonDesktop::sidebarAreaList() const
{
    return d_ptr->sidebarAreaList;
}

QCharList HolonDesktop::sidebarList() const
{
    return d_ptr->sidebarList;
}

void HolonDesktop::addTask(HolonTask* /*task*/)
{

}

void HolonDesktop::addTaskbar(HolonTaskbar *taskbar)
{
    d_ptr->addTaskbar(taskbar);
}

bool HolonDesktop::addSidebar(HolonSidebar* /*sidebar*/)
{
    return true;
}
