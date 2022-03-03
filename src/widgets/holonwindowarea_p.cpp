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
#include "holonnewwindowmenu.h"
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMainWindow>
#include <QDockWidget>

class TitleBar : public QWidget
{
public:
    TitleBar(HolonDesktop *desktop, QDockWidget *parent,
             HolonWindow *window = nullptr,
             HolonWindowAreaPrivate *d = nullptr)
    :   QWidget(parent)
    {
        setStyleSheet(desktop->titleBarStyleSheet());
        setLayout(new QHBoxLayout(this));
        {
            layout()->setContentsMargins({7, 0, 0, 0});
            layout()->setSpacing(0);
            QLabel *label = new QLabel(window ? window->title() : "Open New Window", this);
            {
                label->setFixedHeight(desktop->titleBarHeight());
                layout()->addWidget(label);
            }

            if (!window)
                return;

            auto addButton = [=, this](const QChar &chr)
            {
                QPushButton *button = new QPushButton(chr, this);
                {
                    button->setFixedHeight(desktop->titleBarHeight());
                    button->setFixedWidth(button->height() * 2);
                    button->setFlat(true);
                    button->setStyleSheet(desktop->buttonStyleSheet());
                    layout()->addWidget(button);
                }
                return button;
            };

            QPushButton *maximize = addButton('M');
            {
                connect(maximize, &QPushButton::clicked, this, [=]()
                {
                    d->maximized = !d->maximized;
                    if (d->maximized)
                        maximize->setText("m");
                    else
                        maximize->setText("M");

                     d->maximizeWindow(parent);
                });
            }

            QPushButton *close = addButton('X');
            {
                connect(close, &QPushButton::clicked, this, [=](){ d->closeWindow(parent, window); });
            }
        }
    }
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
    DockWidget(HolonWindowArea *, HolonDesktop *desktop, QMainWindow *parent)
    :   DockWidget(parent)
    {
        setTitleBarWidget(new TitleBar(desktop, this));
        QWidget *widget = new QWidget(this);
        {
            QVBoxLayout *l = new QVBoxLayout(widget);
            {
                widget->setLayout(l);
                HolonNewWindowMenu *menu;
                l->addWidget(menu = new HolonNewWindowMenu(desktop, widget), 0, Qt::AlignCenter);
                connect(menu, &HolonNewWindowMenu::triggered, this, [](HolonWindow *)
                {
                    /* copy section */
                });
            }
            setWidget(widget);
        }
    }

    DockWidget(HolonDesktop *desktop, QMainWindow *parent, HolonWindow *window, HolonWindowAreaPrivate *d)
    :   DockWidget(parent)
    {
        setTitleBarWidget(new TitleBar(desktop, this, window, d));
        setWidget(window->widget());
    }
};

HolonWindowAreaPrivate::HolonWindowAreaPrivate(HolonWindowArea *q, HolonDesktop *desktop)
:   q_ptr(q),
    desktop(desktop),
    mainWindow(new QMainWindow),
    defaultDock(new DockWidget(q, desktop, mainWindow))
{
    mainWindow->setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);
}

HolonWindowAreaPrivate::~HolonWindowAreaPrivate()
{ }

void HolonWindowAreaPrivate::addWindow(HolonWindow *window)
{
    dockList.append(new DockWidget(desktop, mainWindow, window, this));

    if (dockList.count())
        defaultDock->hide();
}

void HolonWindowAreaPrivate::maximizeWindow(QDockWidget *dock)
{
    if (maximized)
    {
        for (QDockWidget *w : dockList)
            w->hide();

        dock->show();
    }
    else
    {
        for (QDockWidget *w : dockList)
            w->show();
    }
}

void HolonWindowAreaPrivate::closeWindow(QDockWidget *dock, HolonWindow *window)
{
    mainWindow->removeDockWidget(dock);
    dock->deleteLater();
    window->deleteLater();

    dockList.removeOne(dock);

    if (!dockList.count())
        defaultDock->show();
    else if(maximized)
    {
        for (QDockWidget *w : dockList)
            w->show();

        maximized = false;
    }
}
