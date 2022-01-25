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
#include "holontaskbar.h"
#include <QRegularExpression>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QVBoxLayout>

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
    QVariant variant = d_ptr->fromString(string);

    if (!variant.isNull())
        return variant;

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

    if (contains("sidebarAreaList"))
        d_ptr->sidebarAreaList = value("sidebarAreaList").toStringList();
    else
    {
        emitError("sidebarAreaList property is not set");
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

    setStyleSheet("QStatusBar { background-color : rgb(64, 66, 68); }"
                  "QStatusBar::item { border: 0px }");
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

bool HolonDesktop::setTaskbar(HolonTaskbar *taskbar)
{
    return d_ptr->setTaskbar(taskbar);
}

bool HolonDesktop::setWorkspace(QWidget *widget)
{
    return d_ptr->setWorkspace(widget);
}

HolonTaskbar *HolonDesktop::taskbar() const
{
    return d_ptr->desktop.taskbar;
}

QWidget *HolonDesktop::workspace() const
{
    return d_ptr->desktop.workspace;
}
