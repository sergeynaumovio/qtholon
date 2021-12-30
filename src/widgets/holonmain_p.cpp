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

#include "holonmain_p.h"
#include "holonmain.h"

#include <QHBoxLayout>
#include <QShortcut>
#include <QStackedWidget>

#define SELECTOR_WIDGET_CHANGED 0

HolonMainPrivate::HolonMainPrivate(HolonMain *q)
:   q_ptr(q)
{ }

bool HolonMainPrivate::mapSidebarArea(QString area)
{
    if (sidebarAreaMap.contains(area))
    {
        if (sidebarAreaMap.value(area))
            return false;
        else
            sidebarAreaMap[area] = true;
    }
    else
        return false;

    return true;
}

bool HolonMainPrivate::mapSidebar(QChar sidebar, QString area, Qt::CheckState checkState)
{
    if (sidebarMap.contains(sidebar))
    {
        if (sidebarMap.value(sidebar))
            return false;
        else
            sidebarMap[sidebar] = true;
    }
    else
        return false;

    sidebarActivator->addSidebarButton(sidebar, area, checkState);

    return true;
}

SidebarButton::SidebarButton(QChar sidebar, QString area, SidebarActivator *parent)
:   QPushButton(sidebar, parent),
    sidebar(sidebar),
    area(area)
{ }

SidebarActivator::SidebarActivator(QWidget *parent)
:   QWidget(parent)
{
    setLayout(new QHBoxLayout(this));
    layout()->setContentsMargins({});
}

void SidebarActivator::addSidebarButton(QChar sidebar, QString area, Qt::CheckState checkState)
{
    SidebarButton *button = new SidebarButton(sidebar, area, this);
    {
        button->setCheckable(true);
        button->setChecked(checkState);
        button->setFlat(true);

        layout()->addWidget(button);

        QObject::connect(button, &QPushButton::clicked, button, [=, this]()
        {
            emit sidebarButtonClicked(sidebar, button->area);
        });

        connect(this, &SidebarActivator::sidebarButtonClicked, button, [button](QChar sidebar, QString area)
        {
            if (button->area == area && button->sidebar != sidebar)
            {
                button->setChecked(false);
            }
        });

        QShortcut *shortcut = new QShortcut(button);
        {
            shortcut->setKey("Alt+" + QString(sidebar));
            connect(shortcut, &QShortcut::activated, button, [button]{ button->click(); });
        }
    }
}

class SidebarLocator : public QWidget
{

#if SELECTOR_WIDGET_CHANGED
    bool once{};

    void set()
    {

    }
#endif

public:
    SidebarLocator(HolonMain *, QWidget *parent)
    :   QWidget(parent)
    {
        setLayout(new QHBoxLayout(this));
        layout()->setContentsMargins({});

#if SELECTOR_WIDGET_CHANGED
        connect(mainWindow, &HolonMain::sidebarSelectorWidgetChanged, this, [=, this](QWidget *widget)
        {
            if (!once && widget == this)
            {
                once = true;

                set();
            }
        });
#endif

    }
};


HBoxWidget::HBoxWidget(HolonMainPrivate *d)
{
    setLayout(new QHBoxLayout(this));
    {
        layout()->setContentsMargins({});
    }

    QPushButton *modeButton = new QPushButton(QIcon(":/holon/screwdriver.svg"),"", this);
    {
        modeButton->setFlat(true);
        modeButton->setCheckable(true);
        layout()->addWidget(modeButton);

        QShortcut *shortcut = new QShortcut(modeButton);
        {
            shortcut->setKey(QKeySequence("Alt+`"));
            connect(shortcut, &QShortcut::activated, modeButton, [modeButton]
            {
                modeButton->click();
            });
        }
    }

    QStackedWidget *sidebarSelector = new QStackedWidget(this);
    {
        layout()->addWidget(sidebarSelector);

        sidebarSelector->layout()->setContentsMargins({});

        d->sidebarActivator = new SidebarActivator(this);
        int activatorIndex = sidebarSelector->addWidget(d->sidebarActivator);
        int locatorIndex = sidebarSelector->addWidget(new SidebarLocator(d->q_ptr, this));

        connect(modeButton, &QPushButton::toggled, sidebarSelector, [=](bool checked)
        {
            if (checked)
                sidebarSelector->setCurrentIndex(locatorIndex);
            else
                sidebarSelector->setCurrentIndex(activatorIndex);

#if SELECTOR_WIDGET_CHANGED
            //emit d_ptr->q_ptr->sidebarSelectorWidgetChanged(sidebarSelector->currentWidget());
#endif
        });
    }

    QPushButton *exit = new QPushButton(QIcon(":/holon/exit.svg"), "", this);
    {
        connect(exit, &QPushButton::clicked, this, [d]()
        {
            d->q_ptr->deleteLater();
        });

        QShortcut *shortcut = new QShortcut(exit);
        {
            shortcut->setKey(QKeySequence::Quit);
            connect(shortcut, &QShortcut::activated, exit, [exit]{ exit->click(); });
        }

        exit->setFlat(true);
        exit->setMaximumWidth(20);
        layout()->addStretch(1);
        layout()->addWidget(exit);
    }
}

QHBoxLayout *HBoxWidget::layout()
{
    return static_cast<QHBoxLayout*>(QWidget::layout());
}
