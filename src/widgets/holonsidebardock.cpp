/****************************************************************************
**
** Copyright (C) 2023 Sergey Naumov
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

#include "holonsidebardock.h"
#include "holondesktop_p.h"
#include "holonmainwindow.h"
#include "holonsidebar.h"
#include "holonsidebardock_p.h"
#include <QDockWidget>
#include <QLabel>
#include <QMainWindow>
#include <QResizeEvent>
#include <QStackedWidget>


void HolonSidebarDock::resizeEvent(QResizeEvent *e)
{
    d.desktop_d().saveDockWidgetWidth(this, e->size().width());
}

HolonSidebarDock::HolonSidebarDock(HolonDesktopPrivate &desktop_d,
                                   const QString &name,
                                   HolonMainWindow *parent)
:   QDockWidget(parent),
    d(*new (&d_storage) HolonSidebarDockPrivate(desktop_d, this, name, parent))
{
    static_assert (sizeof (d_storage) == sizeof (HolonSidebarDockPrivate));
    static_assert (sizeof (ptrdiff_t) == alignof (HolonSidebarDockPrivate));
}

HolonSidebarDock::~HolonSidebarDock()
{ }

void HolonSidebarDock::addSidebar(HolonSidebar *sidebar)
{
    d.stackedWidget()->addWidget(sidebar);
    d.stackedWidget()->setCurrentWidget(sidebar);
}

void HolonSidebarDock::showTitleBarWidget(bool show)
{
    d.showTitleBarWidget(show);
}
