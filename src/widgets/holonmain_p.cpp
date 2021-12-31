/****************************************************************************
**
** Copyright (C) 2021, 2022 Sergey Naumov
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
#include <QSizePolicy>
#include <QButtonGroup>

HolonMainPrivate::HolonMainPrivate(HolonMain *q)
:   q_ptr(q)
{ }

bool HolonMainPrivate::setSidebarArea(QString area)
{
    if (sidebarAreaList.contains(area))
    {
        if (sidebarAreaSet.contains(area))
            return false;
        else
            sidebarAreaSet.insert(area);
    }
    else
        return false;

    return true;
}

bool HolonMainPrivate::setSidebar(QChar sidebar, QString area, Qt::CheckState checkState)
{
    if (sidebarList.contains(sidebar))
    {
        if (sidebarSet.contains(sidebar))
            return false;
        else
            sidebarSet.insert(sidebar);
    }
    else
        return false;

    int index{};
    for (QChar key : sidebarList)
    {
        if (key == sidebar)
            break;

        if (sidebarSet.contains(key))
            ++index;
    }

    sidebarActivator->insertSidebarButton(index, sidebar, area, checkState);

    return true;
}

QHBoxLayout *HBoxWidget::layout()
{
    return static_cast<QHBoxLayout*>(QWidget::layout());
}

HBoxWidget::HBoxWidget(HolonMainPrivate *d, QWidget *parent)
:   QWidget(parent),
    d_ptr(d)
{
    setLayout(new QHBoxLayout(this));
    {
        layout()->setContentsMargins({});
    }
}


SidebarButton::SidebarButton(QChar sidebar, QString area, SidebarActivator *parent)
:   QPushButton(sidebar, parent),
    sidebar(sidebar),
    area(area)
{
    setFlat(true);
    setCheckable(true);
}

SidebarActivator::SidebarActivator(HolonMainPrivate *d, QWidget *parent)
:   HBoxWidget(d, parent)
{ }

void SidebarActivator::insertSidebarButton(int index, QChar sidebar, QString area, Qt::CheckState checkState)
{
    SidebarButton *button = new SidebarButton(sidebar, area, this);
    {
        button->setChecked(checkState);
        layout()->insertWidget(index, button);

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

void SidebarLocator::showEvent(QShowEvent*)
{
    if (once)
        return;

    once = true;

    layout()->addStretch(1);

    for (QChar c : d_ptr->sidebarList)
    {
        QPushButton *sidebarButton = new QPushButton(c, this);
        {
            sidebarButton->setFlat(true);
            sidebarButton->setCheckable(true);
            sidebarButton->setChecked(d_ptr->sidebarSet.contains(c));
            sidebarButton->setMaximumWidth(15);
            layout()->addWidget(sidebarButton);
            QButtonGroup *group = new QButtonGroup(this);
            {
                for (const QString &s : d_ptr->sidebarAreaList)
                {
                    QPushButton *sidebarAreaButton = new QPushButton(s, this);
                    {
                        sidebarAreaButton->setFlat(true);
                        sidebarAreaButton->setCheckable(true);
                        group->addButton(sidebarAreaButton);
                        sidebarAreaButton->setMaximumWidth(15);
                        layout()->addWidget(sidebarAreaButton);

                        connect(sidebarButton, &QPushButton::toggled, sidebarAreaButton, [sidebarAreaButton](bool checked)
                        {
                            sidebarAreaButton->setEnabled(checked);
                        });
                    }
                }
            }
        }

        layout()->addStretch(1);
    }
}

SidebarLocator::SidebarLocator(HolonMainPrivate *d, QWidget *parent)
:   HBoxWidget(d, parent)
{
    layout()->setSpacing(1);
}

SidebarSelector::SidebarSelector(HolonMainPrivate *d)
:   HBoxWidget(d, d->q_ptr)
{
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

        d->sidebarActivator = new SidebarActivator(d, this);
        d->sidebarLocator = new SidebarLocator(d, this);
        int activatorIndex = sidebarSelector->addWidget(d->sidebarActivator);
        int locatorIndex = sidebarSelector->addWidget(d->sidebarLocator);

        connect(modeButton, &QPushButton::toggled, sidebarSelector, [=](bool checked)
        {
            if (checked)
                sidebarSelector->setCurrentIndex(locatorIndex);
            else
                sidebarSelector->setCurrentIndex(activatorIndex);
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
        layout()->addWidget(exit);
    }
}
