// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontitlebar.h"
#include "holonabstractwindow.h"
#include "holondesktop.h"
#include "holondockwidget.h"
#include "holonwindowarea_p.h"
#include <QActionGroup>
#include <QBoxLayout>
#include <QEvent>
#include <QLabel>
#include <QMenu>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>

class HolonTitleBarPrivate
{
public:
    QPushButton *splitButton{};
    QPushButton *maximizeButton{};
    QPushButton *closeButton{};
};

class MenuEventFilter : public QObject
{
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (event->type() == QEvent::MouseButtonRelease)
            if (QMenu *menu = qobject_cast<QMenu *>(watched))
                if (QAction *action = menu->activeAction())
                    if (action->isCheckable())
                    {
                        action->trigger();
                        return true;
                    }

        return QObject::eventFilter(watched, event);
    }

public:
    MenuEventFilter(QObject *parent) : QObject(parent) { }
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
                button->setFixedWidth(button->height() * 1.2);
                button->setFlat(true);
                button->setStyleSheet(desktop->buttonStyleSheet());
                layout()->addWidget(button);
            }
            return button;
        };

        HolonAbstractWindow::Attributes attributes = window->attributes();

        if (attributes.testFlag(HolonAbstractWindow::WindowSplitButtonHint))
        {
            QMenu *menu = new QMenu(parent);

            QAction *split = new QAction(menu);
            split->setText("Split");
            split->setCheckable(true);
            split->setChecked(true);

            QAction *splitSideBySide = new QAction(menu);
            splitSideBySide->setText("Split Side By Side");
            splitSideBySide->setCheckable(true);
            splitSideBySide->setDisabled(true);

            QAction *openNewWidnow = new QAction(menu);
            openNewWidnow->setText("Open in New Window");
            openNewWidnow->setCheckable(true);
            openNewWidnow->setDisabled(true);

            QActionGroup *group = new QActionGroup(menu);
            group->addAction(split);
            group->addAction(splitSideBySide);
            group->addAction(openNewWidnow);

            menu->addActions({split, splitSideBySide, openNewWidnow});
            menu->installEventFilter(new MenuEventFilter(menu));

            menu->addSeparator();

            for (HolonAbstractWindow *second : desktop->windows())
                if (window->area() == second->area())
                {
                    QAction *windowAction = new QAction(second->icon(), second->title(), menu);
                    {
                        menu->addAction(windowAction);
                        connect(windowAction, &QAction::triggered, parent, [=]
                        {
                            desktop->splitWindow(window, second, Qt::Horizontal);
                        });
                    }
                }

            d_ptr->splitButton = addButton('S');
            d_ptr->splitButton->show();
            d_ptr->splitButton->setMenu(menu);
        }

        if (attributes.testFlag(HolonAbstractWindow::WindowMinMaxButtonsHint))
        {
            d_ptr->maximizeButton = addButton('M');
            {
                connect(d_ptr->maximizeButton, &QPushButton::clicked, this, [=, this]()
                {
                    windowarea_d->maximized = !windowarea_d->maximized;
                    if (windowarea_d->maximized)
                    {
                        d_ptr->splitButton->hide();
                        d_ptr->maximizeButton->setText("m");
                    }
                    else
                    {
                        d_ptr->splitButton->show();
                        d_ptr->maximizeButton->setText("M");
                    }

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
