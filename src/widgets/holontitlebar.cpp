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

class HolonTitleBarPrivate
{
public:
    QPushButton *maximizeButton{};
    QPushButton *closeButton{};
};

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
                             HolonWindowAreaPrivate *windowarea_d)
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
                button->hide();
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
            d_ptr->maximizeButton = addButton('M');
            {
                connect(d_ptr->maximizeButton, &QPushButton::clicked, this, [=, this]()
                {
                    windowarea_d->maximized = !windowarea_d->maximized;
                    if (windowarea_d->maximized)
                        d_ptr->maximizeButton->setText("m");
                    else
                        d_ptr->maximizeButton->setText("M");

                    windowarea_d->maximizeWindow(parent);
                });
            }
        }

        if (attributes.testAnyFlag(HolonAbstractWindow::WindowCloseButtonHint))
        {
            d_ptr->closeButton = addButton('X');
            {
                connect(d_ptr->closeButton, &QPushButton::clicked, this, [=](){ desktop->closeWindow(window); });
            }
        }
    }
}

HolonTitleBar::~HolonTitleBar()
{ }

void HolonTitleBar::hideControlButtons()
{
    if (d_ptr->maximizeButton)
        d_ptr->maximizeButton->hide();

    if (d_ptr->closeButton)
        d_ptr->closeButton->hide();
}

void HolonTitleBar::showControlButtons()
{
    if (d_ptr->maximizeButton)
        d_ptr->maximizeButton->show();

    if (d_ptr->closeButton)
        d_ptr->closeButton->show();
}
