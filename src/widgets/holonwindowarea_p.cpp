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

#include "holonwindowarea_p.h"
#include "holondesktop.h"
#include "holonwindow.h"
#include <QBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QDockWidget>

class TitleBar : public QWidget
{
    TitleBar(HolonDesktop *desktop, const QString &title, QDockWidget *parent)
    :   QWidget(parent)
    {
        setStyleSheet(desktop->titleBarStyleSheet());
        setLayout(new QHBoxLayout(this));
        {
            QLabel *label = new QLabel(title, this);
            {
                label->setFixedHeight(desktop->titleBarHeight());
                layout()->addWidget(label);
            }
        }
    }

public:
    TitleBar(HolonDesktop *desktop, QDockWidget *parent)
    :   TitleBar(desktop, "", parent)
    { }

    TitleBar(HolonDesktop *desktop, HolonWindow *window, QDockWidget *parent)
    :   TitleBar(desktop, window->title(), parent)
    { }
};

class DockWidget : public QDockWidget
{
    DockWidget(QMainWindow *parent)
    :   QDockWidget(parent)
    {
        parent->addDockWidget(Qt::LeftDockWidgetArea, this);
        setFeatures(QDockWidget::NoDockWidgetFeatures);
    }

public:
    DockWidget(HolonDesktop *desktop, QMainWindow *parent)
    :   DockWidget(parent)
    {
        setTitleBarWidget(new TitleBar(desktop, this));
        setWidget(new QWidget(this));
    }

    DockWidget(HolonDesktop *desktop, HolonWindow *window, QMainWindow *parent)
    :   DockWidget(parent)
    {
        setTitleBarWidget(new TitleBar(desktop, window, this));
        setWidget(window->widget());
    }
};

HolonWindowAreaPrivate::HolonWindowAreaPrivate(HolonDesktop *desktop)
:   desktop(desktop),
    mainWindow(new QMainWindow),
    defaultDock(new DockWidget(desktop, mainWindow))
{
    mainWindow->setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);
}

HolonWindowAreaPrivate::~HolonWindowAreaPrivate()
{ }

void HolonWindowAreaPrivate::addWindow(HolonWindow *window)
{
    new DockWidget(desktop, window, mainWindow);

    if (!count)
        defaultDock->hide();

    ++count;
}
