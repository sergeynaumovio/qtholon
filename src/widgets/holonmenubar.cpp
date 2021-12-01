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

#include "holonmenubar.h"
#include "holonmainwindow.h"
#include <QLoaderSettings>
#include <QMenu>
#include <QAction>

HolonMenuBar::HolonMenuBar(QLoaderSettings *settings, HolonMainWindow *parent)
:   QMenuBar(parent),
    QLoaderSettings(settings)
{
    parent->setMenuBar(this);

    QMenu *file = new QMenu("&File", this);
    addMenu(file);

    QAction *exit = new QAction("E&xit", file);
    file->addAction(exit);

    connect(exit, &QAction::triggered, this, [this] { mainWindow()->deleteLater(); });
}

HolonMainWindow *HolonMenuBar::mainWindow() const
{
    return static_cast<HolonMainWindow*>(parent());
}
