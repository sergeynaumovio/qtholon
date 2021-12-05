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
#include "holonwidgetinterface.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QPushButton>

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

    connect(this, &HolonMain::sidebarAdded, menu, [menu](QString name)
    {
        menu->addAction(name);
    });

    setStatusBar(statusbar);

// -----------------------------------------------------------------------------

    if (!parent)
        show();
}

HolonMain::~HolonMain()
{ }

void HolonMain::addWidget(HolonWidgetInterface *widget)
{
    QString title = widget->property("title").toString();
    d_ptr->widgets[title] = widget;
}
