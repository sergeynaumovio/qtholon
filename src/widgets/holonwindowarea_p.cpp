// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindowarea_p.h"
#include "holonwindowarea.h"
#include "holondesktop.h"
#include "holonwindow.h"
#include "holonwindowmenu.h"
#include <QBoxLayout>
#include <QDockWidget>
#include <QLabel>
#include <QLoaderTree>
#include <QMainWindow>
#include <QPushButton>

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
    DockWidget(HolonDesktop *desktop, HolonWindowArea *area, QMainWindow *parent)
    :   DockWidget(parent)
    {
        setTitleBarWidget(new TitleBar(desktop, this));
        QWidget *widget = new QWidget(this);
        {
            QVBoxLayout *l = new QVBoxLayout(widget);
            {
                widget->setLayout(l);
                HolonWindowMenu *menu;
                l->addWidget(menu = new HolonWindowMenu(desktop, widget), 0, Qt::AlignCenter);
                connect(menu, &HolonWindowMenu::triggered, this, [area](HolonWindow *window)
                {
                    QStringList to = area->section();
                    to.append(qAsConst(window)->section().last());
                    area->tree()->copy(window->section(), to);
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

HolonWindowAreaPrivate::HolonWindowAreaPrivate(HolonDesktop *desk, HolonWindowArea *q)
:   desktop(desk),
    q_ptr(q),
    mainWindow(new QMainWindow),
    defaultDock(new DockWidget(desktop, q, mainWindow))
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
