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

#include "holonsidebarstack.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include "holonsidebardock_p.h"
#include "holonmainwindow.h"
#include <QBoxLayout>
#include <QDockWidget>
#include <QLabel>
#include <QMainWindow>
#include <QStackedWidget>

class HolonSidebarStackTitleBar : public QWidget
{
public:
    HolonSidebarStackTitleBar(HolonDesktop *desktop, QDockWidget *parent)
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

class HolonSidebarStackPrivate : public HolonSidebarDockPrivate
{
public:
    QStackedWidget *widget;
    QWidget *currentWidget;

    HolonSidebarStackPrivate(HolonDesktop *desktop)
    :   HolonSidebarDockPrivate(desktop)
    { }
};

void HolonSidebarStack::resizeEvent(QResizeEvent *)
{
    Q_D(HolonSidebarStack);

    if (d->sidebarAreasAdded)
        emit d->mainWindow->layoutChanged();
}

HolonSidebarStack::HolonSidebarStack(QLoaderSettings *settings, HolonDesktop *desktop)
:   HolonSidebarDock(*new HolonSidebarStackPrivate(desktop), settings->section().constLast()),
    QLoaderSettings(settings)
{
    Q_D(HolonSidebarStack);

    d->titlebar.hidden = new QWidget(this);
    d->widget = new QStackedWidget(this);
    QString name = section().constLast();

    QLabel *label = new QLabel(this);
    {
        label->setText(name);
        QFont font("Arial", 20, QFont::Bold);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        d->widget->addWidget(label);
    }

    setFeatures(QDockWidget::DockWidgetMovable);
    setObjectName(name);
    setTitleBarWidget(d->titlebar.hidden);
    setWidget(d->widget);

    connect(this, &QDockWidget::dockLocationChanged, desktop, [desktop]
    {
        emit desktop->mainWindow()->layoutChanged();
    });
}

HolonSidebarStack::~HolonSidebarStack()
{ }

void HolonSidebarStack::addSidebar(HolonSidebar *sidebar)
{
    Q_D(HolonSidebarStack);

    d->widget->addWidget(sidebar);
    d->widget->setCurrentWidget(sidebar);
}

HolonDesktop *HolonSidebarStack::desktop() const
{
    Q_D(const HolonSidebarDock);

    return d->desktop;
}

void HolonSidebarStack::setSidebarAreasAdded(bool added)
{
    Q_D(HolonSidebarDock);

    d->sidebarAreasAdded = added;
}

void HolonSidebarStack::showTitleBarWidget(bool show)
{
    Q_D(HolonSidebarStack);

    if (show)
    {
        d->currentWidget = d->widget->currentWidget();
        d->widget->setCurrentIndex(0);
        setTitleBarWidget(d->titlebar.visible = new HolonSidebarStackTitleBar(d->desktop, this));
        d->titlebar.hidden->deleteLater();
    }
    else
    {
        d->widget->setCurrentWidget(d->currentWidget);
        setTitleBarWidget(d->titlebar.hidden = new QWidget(this));
        d->titlebar.visible->deleteLater();
    }
}
