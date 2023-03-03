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
:   QWidget(parent),
    d(*new (&d_storage) HolonTitleBarPrivate)
{
    static_assert (sizeof (d_storage) == sizeof (HolonTitleBarPrivate));
    static_assert (sizeof (ptrdiff_t) == alignof (HolonTitleBarPrivate));

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
            d.maximizeButton = addButton('M');
            {
                connect(d.maximizeButton, &QPushButton::clicked, this, [=, this]()
                {
                    windowarea_d->maximized = !windowarea_d->maximized;
                    if (windowarea_d->maximized)
                        d.maximizeButton->setText("m");
                    else
                        d.maximizeButton->setText("M");

                    windowarea_d->maximizeWindow(parent);
                });
            }
        }

        if (attributes.testAnyFlag(HolonAbstractWindow::WindowCloseButtonHint))
        {
            d.closeButton = addButton('X');
            {
                connect(d.closeButton, &QPushButton::clicked, this, [=](){ desktop->closeWindow(window); });
            }
        }
    }
}

void HolonTitleBar::hideControlButtons()
{
    if (d.maximizeButton)
        d.maximizeButton->hide();

    if (d.closeButton)
        d.closeButton->hide();
}

void HolonTitleBar::showControlButtons()
{
    if (d.maximizeButton)
        d.maximizeButton->show();

    if (d.closeButton)
        d.closeButton->show();
}
