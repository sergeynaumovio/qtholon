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

#include "holonwindow.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include <QIcon>

HolonWindow::HolonWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   QObject(parent),
    QLoaderSettings(settings)
{
    parent->addWindow(this);
}

HolonWindow::HolonWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   QObject(parent),
    QLoaderSettings(settings)
{ }

HolonWindow::Areas HolonWindow::areas() const { return {}; }
QIcon HolonWindow::icon() const { return {}; }
QString HolonWindow::title() const { return {}; }
QWidget *HolonWindow::toolbar() const { return {}; }
QWidget *HolonWindow::widget() const { return {}; }
