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
#include "holondesktop.h"
#include "holontaskbar.h"
#include "holonwindow.h"
#include <QBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QDockWidget>
#include <QLoaderTree>

class HolonSidebarDockWidgetTitleBar : public QWidget
{
public:
    HolonSidebarDockWidgetTitleBar(HolonDesktop *desktop, QDockWidget *parent)
    :   QWidget(parent)
    {
        setStyleSheet(desktop->titleBarStyleSheet());
        setLayout(new QHBoxLayout(this));
        {
            QLabel *label = new QLabel("", this);
            {
                label->setFixedHeight(desktop->titleBarHeight());
                layout()->addWidget(label);
            }
        }
    }
};

class HolonSidebarDockWidget : public QDockWidget
{
public:
    HolonSidebarDockWidget(HolonDesktop *desktop, QMainWindow *parent)
    :   QDockWidget(parent)
    {
        parent->addDockWidget(Qt::LeftDockWidgetArea, this);
        setFeatures(QDockWidget::NoDockWidgetFeatures);
        setTitleBarWidget(new HolonSidebarDockWidgetTitleBar(desktop, this));
        setWidget(new QWidget(this));
    }

    HolonSidebarDockWidget(HolonDesktop *desktop, HolonWindow *window, QMainWindow *parent)
    :   QDockWidget(parent)
    {
        parent->addDockWidget(Qt::LeftDockWidgetArea, this);
        setFeatures(QDockWidget::NoDockWidgetFeatures);
        setTitleBarWidget(new HolonSidebarDockWidgetTitleBar(desktop, this));
        setWidget(window->widget());
    }
};

class HolonSidebarPrivate
{
public:
    HolonDesktop *const desktop;
    QMainWindow *const mainWindow;
    QDockWidget *const defaultDock;
    const QChar sidebar;
    const QString sidebarArea;
    int count{};

    HolonSidebarPrivate(HolonSidebar *q_ptr,
                        HolonDesktop *desktop_ptr,
                        const QChar sidebar_name,
                        const QString &sidebarArea_name)
    :   desktop(desktop_ptr),
        mainWindow(new QMainWindow),
        defaultDock(new HolonSidebarDockWidget(desktop_ptr, mainWindow)),
        sidebar(sidebar_name),
        sidebarArea(sidebarArea_name)
    {
        mainWindow->setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);
        mainWindow->setParent(q_ptr);
    }

    void addWindow(HolonWindow *window)
    {
        new HolonSidebarDockWidget(desktop, window, mainWindow);

        if (!count)
            defaultDock->hide();

        ++count;
    }
};

HolonSidebar::HolonSidebar(QLoaderSettings *settings, HolonDesktop *desktop)
:   QWidget(desktop),
    QLoaderSettings(settings),
    d_ptr(new HolonSidebarPrivate(this,
                                  desktop,
                                  value("sidebar").toChar(),
                                  value("sidebarArea").toString()))
{
    if (!desktop->sidebarList().contains(d_ptr->sidebar))
    {
        emitError("sidebar is not in list");
        return;
    }

    if (!desktop->sidebarAreaList().contains(d_ptr->sidebarArea))
    {
        emitError("sidebarArea is not in list");
        return;
    }

    if (!desktop->addSidebar(this))
    {
        emitError("sidebar is already added");
        return;
    }

    setLayout(new QHBoxLayout(this));
    {
        layout()->addWidget(d_ptr->mainWindow);
        layout()->setContentsMargins({});
    }
}

HolonSidebar::~HolonSidebar()
{ }

void HolonSidebar::addWindow(HolonWindow *window)
{
    d_ptr->addWindow(window);
}

QChar HolonSidebar::sidebar() const
{
    return d_ptr->sidebar;
}

QString HolonSidebar::sidebarArea() const
{
    return d_ptr->sidebarArea;
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
