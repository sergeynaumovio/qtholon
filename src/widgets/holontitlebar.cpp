// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontitlebar.h"
#include "holonabstractwindow.h"
#include "holondesktop.h"
#include "holondockwidget.h"
#include "holonwindowarea_p.h"
#include <QBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>

void HolonTitleBar::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

HolonTitleBar::HolonTitleBar(HolonDesktop *desktop,
                             HolonDockWidget *parent,
                             HolonAbstractWindow *window,
                             HolonWindowAreaPrivate *d)
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
                connect(close, &QPushButton::clicked, this, [=](){ desktop->closeWindow(window); });
            }
        }
    }
}
