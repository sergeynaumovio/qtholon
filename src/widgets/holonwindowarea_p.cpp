// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindowarea_p.h"
#include "holonabstractwindow.h"
#include "holonwindowarea.h"
#include "holondesktop.h"
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
             HolonAbstractWindow *window = nullptr,
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

            HolonAbstractWindow::Attributes attributes = window->attributes();
            if (attributes.testFlag(HolonAbstractWindow::WindowMinMaxButtonsHint))
            {
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
            }

            if (attributes.testAnyFlag(HolonAbstractWindow::WindowCloseButtonHint))
            {
                QPushButton *close = addButton('X');
                {
                    connect(close, &QPushButton::clicked, this, [=](){ d->closeWindow(window); });
                }
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
                connect(menu, &HolonWindowMenu::triggered, this, [area](HolonAbstractWindow *window)
                {
                    QStringList to = area->section();
                    to.append(qAsConst(window)->section().last());
                    area->tree()->copy(window->section(), to);
                });
            }
            setWidget(widget);
        }
    }

    DockWidget(HolonDesktop *desktop, QMainWindow *parent, HolonAbstractWindow *window, HolonWindowAreaPrivate *d)
    :   DockWidget(parent)
    {
        setTitleBarWidget(new TitleBar(desktop, this, window, d));
        setWidget(window->widget());
        widget()->setFocus();
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

void HolonWindowAreaPrivate::addWindow(HolonAbstractWindow *window)
{
    QDockWidget *dock = new DockWidget(desktop, mainWindow, window, this);
    dockList.append(dock);

    if (dockList.count())
        defaultDock->hide();

    dockByWindow.insert(window, dock);
    desktop->addWindow(window);
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

void HolonWindowAreaPrivate::closeWindow(HolonAbstractWindow *window)
{
    QDockWidget *dock = dockByWindow.value(window);
    dockByWindow.remove(window);
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

void HolonWindowAreaPrivate::setChecked(bool checked)
{
    q_ptr->setValue("checked", checked);
}
