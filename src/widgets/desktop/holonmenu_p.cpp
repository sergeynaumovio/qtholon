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

#include "holonmenu_p.h"
#include "holonmenu.h"
#include "holontaskbar.h"
#include "holondesktop.h"
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QShortcut>
#include <QLoaderTree>

HolonMenuPrivate::HolonMenuPrivate(HolonMenu *q, HolonTaskbar *parent)
:   q_ptr(q),
    taskbar(parent),
    menu(new HolonMenuWidget(this, parent->desktop()))
{ }

void HolonMenuWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

HolonMenuWidget::HolonMenuWidget(HolonMenuPrivate *d, HolonDesktop *parent)
:   QWidget(parent),
    d_ptr(d)
{
    setLayout(new QVBoxLayout(this));
    setStyleSheet(d_ptr->taskbar->styleSheet());

    connect(d->q_ptr, &QPushButton::clicked, this, [this]()
    {
        if (isVisible())
            hide();
        else
            show();
    });

    connect(d->taskbar->desktop(), &HolonDesktop::sizeChanged, this, [this](QSize size)
    {
        move(x(), size.height() - d_ptr->taskbar->preferedHeight() - height());
    });

    QPushButton *quitButton = new QPushButton("Quit", this);
    {
        connect(quitButton, &QPushButton::clicked, this, [d]()
        {
            d->taskbar->desktop()->deleteLater();
        });

        QShortcut *shortcut = new QShortcut(d->taskbar->desktop());
        {
            shortcut->setKey(QKeySequence("Ctrl+Q"));
            connect(shortcut, &QShortcut::activated, quitButton, [=, this]
            {
                d_ptr->q_ptr->tree()->save();
                quitButton->click();
            });
        }

        quitButton->setFlat(true);
        layout()->addWidget(quitButton);
    }
}
