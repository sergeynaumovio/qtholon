/****************************************************************************
**
** Copyright (C) 2021 Sergey Naumov
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

#include "holonmain.h"
#include "holonwidgetinterface.h"
#include <QHBoxLayout>
#include <QStatusBar>
#include <QPushButton>
#include <QMenu>
#include <QHash>

class HolonWidgetInterface;

class HolonMainPrivate
{
public:
    HolonMain *const q_ptr;
    QHash<QString, HolonWidgetInterface*> widgets;

    HolonMainPrivate(HolonMain *q);
};

HolonMainPrivate::HolonMainPrivate(HolonMain *q)
:   q_ptr(q)
{ }

class HBoxWidget : public QWidget
{
    QHBoxLayout *layout()
    {
        return static_cast<QHBoxLayout*>(QWidget::layout());
    }

public:
    HBoxWidget(HolonMain *mainWindow)
    {
        setLayout(new QHBoxLayout(this));
        {
            layout()->setContentsMargins({});
        }

        QPushButton *sidebar = new QPushButton("\u2261", this);
        {
            QMenu *menu = new QMenu(sidebar);
            {
                sidebar->setMenu(menu);
                connect(mainWindow, &HolonMain::sidebarAdded, menu, [menu](QString name)
                {
                    menu->addAction(name);
                });
            }

            sidebar->setFlat(true);
            layout()->addWidget(sidebar);
        }

        QPushButton *exit = new QPushButton(QIcon(":/holon/exit.svg"), "", this);
        {
            connect(exit, &QPushButton::clicked, this, [mainWindow]()
            {
                mainWindow->deleteLater();
            });

            exit->setFlat(true);
            exit->setMaximumWidth(20);
            layout()->addStretch(1);
            layout()->addWidget(exit);
        }
    }
};

void HolonMain::addWidget(HolonWidgetInterface *widget)
{
    QString title = widget->property("title").toString();
    d_ptr->widgets[title] = widget;
}

void HolonMain::closeEvent(QCloseEvent*)
{
    hide();
    deleteLater();
}

HolonMain::HolonMain(QLoaderSettings *settings, QWidget *parent)
:   QLoaderSettings(settings),
    d_ptr(new HolonMainPrivate(this))
{
    setParent(parent);

    if (!parent)
        show();

    statusBar()->addWidget(new HBoxWidget(this), 1);

    setStyleSheet("QStatusBar { background-color : rgb(64, 66, 68) }"
                  "QStatusBar::item { border: 0px }" ) ;
}

HolonMain::~HolonMain()
{ }

