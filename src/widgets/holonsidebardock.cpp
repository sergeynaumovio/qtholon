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
    HolonSidebarDockTitleBar(HolonDesktop *desktop, QDockWidget *parent)
    :   QWidget(parent)
    {
        setStyleSheet(desktop->titleBarStyleSheet());
        setLayout(new QHBoxLayout(this));
        {
            layout()->setContentsMargins({});
            QLabel *label = new QLabel("", this);
            {
                label->setFixedHeight(desktop->titleBarHeight());
                layout()->addWidget(label);
            }
        }
    }
};

class HolonSidebarDockPrivate
{
public:
    HolonDesktop *const desktop;
    HolonMainWindow *const mainWindow;

    struct
    {
        HolonSidebarDockTitleBar *visible;
        QWidget *hidden;

    } titlebar;

    QStackedWidget *stackedWidget;
    QWidget *currentWidget;

    HolonSidebarDockPrivate(HolonDesktop *desk, HolonMainWindow *parent)
    :   desktop(desk),
        mainWindow(parent)
    { }
};

void HolonSidebarDock::resizeEvent(QResizeEvent *)
{
    emit d_ptr->mainWindow->layoutChanged();
}

HolonSidebarDock::HolonSidebarDock(const QString &name,
                                   HolonDesktop *desktop,
                                   HolonMainWindow *parent)
:   QDockWidget(parent),
    d_ptr(new HolonSidebarDockPrivate(desktop, parent))
{
    d_ptr->titlebar.hidden = new QWidget(this);
    d_ptr->stackedWidget = new QStackedWidget(this);

    QLabel *label = new QLabel(this);
    {
        label->setText(name);
        QFont font("Arial", 20, QFont::Bold);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        d_ptr->stackedWidget->addWidget(label);
    }

    setFeatures(QDockWidget::DockWidgetMovable);
    setObjectName(name);
    setTitleBarWidget(d_ptr->titlebar.hidden);
    setWidget(d_ptr->stackedWidget);
}

HolonSidebarDock::~HolonSidebarDock()
{ }

void HolonSidebarDock::addSidebar(HolonSidebar *sidebar)
{
    d_ptr->stackedWidget->addWidget(sidebar);
    d_ptr->stackedWidget->setCurrentWidget(sidebar);
}

void HolonSidebarDock::showTitleBarWidget(bool show)
{
    if (show)
    {
        d_ptr->currentWidget = d_ptr->stackedWidget->currentWidget();
        d_ptr->stackedWidget->setCurrentIndex(0);
        setTitleBarWidget(d_ptr->titlebar.visible = new HolonSidebarDockTitleBar(d_ptr->desktop, this));
        d_ptr->titlebar.hidden->deleteLater();
    }
    else
    {
        d_ptr->stackedWidget->setCurrentWidget(d_ptr->currentWidget);
        setTitleBarWidget(d_ptr->titlebar.hidden = new QWidget(this));
        d_ptr->titlebar.visible->deleteLater();
    }
}
