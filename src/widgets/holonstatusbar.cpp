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

#include "holonstatusbar.h"
#include "holonmain.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>

class HBoxWidget : public QWidget
{
public:
    HBoxWidget(QStatusBar *parent)
    :   QWidget(parent)
    {
        setLayout(new QHBoxLayout(this));
        layout()->setContentsMargins({});
        parent->addWidget(this, 1);
        parent->setStyleSheet("QStatusBar { background-color : rgb(64, 66, 68) }"
                              "QStatusBar::item { border: 0px }" ) ;
    }

    QHBoxLayout *layout()
    {
        return static_cast<QHBoxLayout*>(QWidget::layout());
    }
};

void HolonStatusBar::addSidebarButton(QPushButton *button)
{
    button->setParent(widget);
    button->setFlat(true);
    widget->layout()->addWidget(button);
}

void HolonStatusBar::addExitButton(QPushButton *button)
{
    button->setParent(widget);
    button->setFlat(true);
    button->setMaximumWidth(20);
    widget->layout()->addStretch(1);
    widget->layout()->addWidget(button);
}

HolonStatusBar::HolonStatusBar(HolonMain *parent)
:   QStatusBar(parent),
    widget(new HBoxWidget(this))
{
    if (!parent)
        return;

    QPushButton *sidebar = new QPushButton("\u2261");
    {
        QMenu *menu = new QMenu(sidebar);
        {
            sidebar->setMenu(menu);
            connect(parent, &HolonMain::sidebarAdded, menu, [menu](QString name)
            {
                menu->addAction(name);
            });
        }
        addSidebarButton(sidebar);
    }

    QPushButton *exit = new QPushButton(QIcon(":/holon/exit.svg"), "");
    {
        connect(exit, &QPushButton::clicked, this, [parent]()
        {
            parent->deleteLater();
        });

        addExitButton(exit);
    }
}
