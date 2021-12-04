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

#include "holonmainwindow.h"
#include "holonmainwindow_p.h"
#include "holonareasidebar.h"
#include "holonsidewidget.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QPushButton>

void HolonMainWindow::closeEvent(QCloseEvent*)
{
    hide();
    deleteLater();
}

HolonMainWindow::HolonMainWindow(QLoaderSettings *settings, QWidget *parent)
:   QLoaderSettings(settings),
    d_ptr(new HolonMainWindowPrivate(this))
{
    setParent(parent);

// Menu Bar --------------------------------------------------------------------
    QMenuBar *menubar = new QMenuBar(this);

    QMenu *file = new QMenu("&File", menubar);
    menubar->addMenu(file);

    QAction *exit = new QAction("E&xit", file);
    file->addAction(exit);

    connect(exit, &QAction::triggered, menubar, [this]
    {
        deleteLater();
    });

    setMenuBar(menubar);

// Status Bar ------------------------------------------------------------------
    QStatusBar *statusbar = new QStatusBar(this);
    QPushButton *button = new QPushButton("\u2261", statusbar);
    button->setFlat(true);
    statusbar->addWidget(button);

    QMenu *menu = new QMenu(button);
    button->setMenu(menu);

    setStatusBar(statusbar);

// -----------------------------------------------------------------------------

    if (!parent)
        show();
}

HolonMainWindow::~HolonMainWindow()
{ }

void HolonMainWindow::addSideBar(HolonAreaSideBar *area)
{
    QString side = area->property("area").toString();
    if (side == "top")
    {
    }
    else if (side == "right")
    {
    }
    else if (side == "bottom")
    {
    }
    else
    {
    }
}

void HolonMainWindow::addSideWidget(HolonSideWidget *widget)
{
    QString title = widget->property("title").toString();
    d_ptr->sidewidgets[title] = widget;
}
