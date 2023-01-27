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
#include "holondesktop.h"
#include "holondesktop_p.h"
#include "holonmainwindow.h"
#include "holonsidebar.h"
#include <QBoxLayout>
#include <QDockWidget>
#include <QLabel>
#include <QMainWindow>
#include <QStackedWidget>

class HolonSidebarDockTitleBar : public QWidget
{
public:
    HolonSidebarDockTitleBar(HolonDesktopPrivate &desktop_d, QDockWidget *parent)
    :   QWidget(parent)
    {
        setStyleSheet(desktop_d.titleBarStyleSheet());
        setLayout(new QHBoxLayout(this));
        {
            layout()->setContentsMargins({});
            QLabel *label = new QLabel("", this);
            {
                label->setFixedHeight(desktop_d.titleBarHeight());
                layout()->addWidget(label);
            }
        }
    }
};

class HolonSidebarDockPrivate
{
public:
    HolonDesktopPrivate &desktop_d;
    HolonMainWindow *const mainWindow;

    struct
    {
        HolonSidebarDockTitleBar *visible;
        QWidget *hidden;

    } titlebar;

    QStackedWidget *stackedWidget;
    QWidget *currentWidget;

    HolonSidebarDockPrivate(HolonDesktopPrivate &desk_d,
                            HolonMainWindow *parent)
    :   desktop_d(desk_d),
        mainWindow(parent)
    { }
};

void HolonSidebarDock::resizeEvent(QResizeEvent *)
{
    d.desktop_d.saveState();
}

HolonSidebarDock::HolonSidebarDock(HolonDesktopPrivate &desktop_d,
                                   const QString &name,
                                   HolonMainWindow *parent)
:   QDockWidget(parent),
    d(*new (&d_storage) HolonSidebarDockPrivate(desktop_d, parent))
{
    static_assert (sizeof (d_storage) == sizeof (HolonSidebarDockPrivate));
    static_assert (sizeof (ptrdiff_t) == alignof (HolonSidebarDockPrivate));

    d.titlebar.hidden = new QWidget(this);
    d.stackedWidget = new QStackedWidget(this);

    QLabel *label = new QLabel(this);
    {
        label->setText(name);
        QFont font("Arial", 20, QFont::Bold);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        d.stackedWidget->addWidget(label);
    }

    setFeatures(QDockWidget::DockWidgetMovable);
    setObjectName(name);
    setTitleBarWidget(d.titlebar.hidden);
    setWidget(d.stackedWidget);
}

HolonSidebarDock::~HolonSidebarDock()
{ }

void HolonSidebarDock::addSidebar(HolonSidebar *sidebar)
{
    d.stackedWidget->addWidget(sidebar);
    d.stackedWidget->setCurrentWidget(sidebar);
}

void HolonSidebarDock::showTitleBarWidget(bool show)
{
    if (show)
    {
        d.currentWidget = d.stackedWidget->currentWidget();
        d.stackedWidget->setCurrentIndex(0);
        setTitleBarWidget(d.titlebar.visible = new HolonSidebarDockTitleBar(d.desktop_d, this));
        d.titlebar.hidden->deleteLater();
    }
    else
    {
        d.stackedWidget->setCurrentWidget(d.currentWidget);
        setTitleBarWidget(d.titlebar.hidden = new QWidget(this));
        d.titlebar.visible->deleteLater();
    }
}
