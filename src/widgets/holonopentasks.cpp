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

#include "holonopentasks.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include <QBoxLayout>
#include <QLabel>
#include <QIcon>

class HolonOpenTasksWindowPrivate
{
public:
    QWidget *widget{};
};

HolonOpenTasksWindow::HolonOpenTasksWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonWindow(settings, parent)
{ }

HolonOpenTasksWindow::HolonOpenTasksWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonWindow(settings, parent),
    d_ptr(new HolonOpenTasksWindowPrivate)
{
    parent->addWindow(this);
}

HolonOpenTasksWindow::~HolonOpenTasksWindow()
{}

QIcon HolonOpenTasksWindow::icon() const
{
    return {};
}

QString HolonOpenTasksWindow::title() const
{
    return {};
}

QWidget *HolonOpenTasksWindow::toolbar() const
{
    return {};
}

QWidget *HolonOpenTasksWindow::widget() const
{
    QWidget *widget = new QWidget(d_ptr->widget);
    {
        widget->setLayout(new QVBoxLayout(widget));
        {
            widget->layout()->addWidget(new QLabel("Open Tasks", widget));
        }
    }

    return widget;
}
