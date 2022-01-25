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

#include "holondesktop_p.h"
#include "holondesktop.h"
#include "holontaskbar.h"
#include "holonsidebararea.h"
#include "holonsidebar.h"

#include <QLoaderTree>
#include <QHBoxLayout>
#include <QShortcut>
#include <QStackedWidget>
#include <QSizePolicy>
#include <QButtonGroup>

HolonDesktopPrivate::HolonDesktopPrivate(HolonDesktop *q)
:   charlist("^QCharList\\s*\\(\\s*(?<list>.*)\\)"),
    q_ptr(q),
    tree(q->tree())
{ }

QVariant HolonDesktopPrivate::fromString(const QString &value) const
{
    QRegularExpressionMatch match;
    QVariant variant;

    if ((match = charlist.match(value)).hasMatch())
    {
        QStringList stringlist = match.captured("list").split(',');
        QCharList list;

        for (QString &string : stringlist)
        {
            string = string.trimmed();
            if (string.size() != 1)
                return QVariant();

            list.append(string.at(0));
        }

        variant.setValue(list);

        return variant;
    }

    return {};
}

bool HolonDesktopPrivate::mapSidebarArea(QString area, HolonSidebarArea *q)
{
    if (sidebarAreaList.contains(area))
    {
        if (sidebarAreas.contains(area))
            return false;
        else
            sidebarAreas.insert(area, q);
    }
    else
        return false;

    return true;
}

bool HolonDesktopPrivate::mapSidebar(QPair<QChar, HolonSidebar*> sidebar,
                                     QPair<QString, HolonSidebarArea*> area, Qt::CheckState checkState)
{
    QChar c = sidebar.first;
    QString s = area.first;

    if (sidebarList.contains(c))
    {
        if (sidebarRelatedObjects.contains(c))
            return false;
        else
            sidebarRelatedObjects.insert(c, {area.second, sidebar.second, nullptr});
    }
    else
        return false;

    int index{};
    for (QChar key : sidebarList)
    {
        if (key == c)
            break;

        if (sidebarRelatedObjects.contains(key))
            ++index;
    }

    sidebarActivator->insertSidebarButton(index, c, s, checkState);

    return true;
}

bool HolonDesktopPrivate::setTaskbar(HolonTaskbar *taskbar)
{
    if (desktop.taskbar)
    {
        q_ptr->emitError("Desktop taskbar is already set");
        return false;
    }

    QWidget *central = q_ptr->centralWidget();

    if (!central)
    {
        QWidget *widget = new QWidget(q_ptr);
        {
            static_cast<QMainWindow*>(q_ptr)->setCentralWidget(widget);

            QBoxLayout *layout;
            {
                if (taskbar->area() == HolonTaskbar::Left ||
                    taskbar->area() == HolonTaskbar::Right)
                {
                    layout = new QBoxLayout(QBoxLayout::LeftToRight, widget);
                }
                else
                {
                    layout = new QBoxLayout(QBoxLayout::TopToBottom, widget);
                }

                widget->setLayout(layout);
                layout->setContentsMargins({});
                layout->setSpacing(0);
                layout->addWidget(taskbar);
            }
        }
    }

    desktop.taskbar = taskbar;

    return true;
}

bool HolonDesktopPrivate::setWorkspace(QWidget *widget)
{
    if (desktop.workspace)
    {
        q_ptr->emitError("Desktop workspace is already set");
        return false;
    }

    QWidget *central = q_ptr->centralWidget();

    if (central)
    {
        if (desktop.taskbar)
        {
            if (desktop.taskbar->area() == HolonTaskbar::Left ||
                desktop.taskbar->area() == HolonTaskbar::Top)
            {
                central->layout()->addWidget(widget);
            }
            else
            {
                static_cast<QBoxLayout*>(central->layout())->insertWidget(0, widget);
            }
        }
    }

    desktop.workspace = widget;

    return true;
}

HBoxWidget::Layout *HBoxWidget::layout()
{
    return static_cast<Layout*>(QWidget::layout());
}

HBoxWidget::HBoxWidget(HolonDesktopPrivate *d, QWidget *parent)
:   QWidget(parent),
    d_ptr(d)
{
    setLayout(new Layout(this));
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

SidebarActivator::SidebarActivator(HolonDesktopPrivate *d, QWidget *parent)
:   HBoxWidget(d, parent)
{ }

void SidebarActivator::insertSidebarButton(int index, QChar sidebar, QString area, Qt::CheckState checkState)
{
    SidebarButton *button = new SidebarButton(sidebar, area, this);
    {
        d_ptr->sidebarRelatedObjects[sidebar].button = button;
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
            sidebarButton->setChecked(d_ptr->sidebarRelatedObjects.contains(c));
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
                        sidebarAreaButton->setChecked(d_ptr->sidebarRelatedObjects.contains(c) &&
                                                      d_ptr->sidebarRelatedObjects.value(c).area->objectName() == s);
                        group->addButton(sidebarAreaButton);
                        sidebarAreaButton->setMaximumWidth(15);
                        layout()->addWidget(sidebarAreaButton);

                        connect(sidebarButton, &QPushButton::toggled, sidebarAreaButton, [sidebarAreaButton](bool checked)
                        {
                            sidebarAreaButton->setEnabled(checked);
                        });

                        connect(sidebarAreaButton, &QPushButton::toggled, sidebarAreaButton, [=, this](bool checked)
                        {
                            if (checked)
                            {
                                SidebarRelatedObjects &sidebarObjects = d_ptr->sidebarRelatedObjects[c];
                                HolonSidebar *sidebar = sidebarObjects.sidebar;
                                HolonSidebarArea *next = d_ptr->sidebarAreas.value(s);

                                next->addSidebar(sidebar);
                            }
                        });
                    }
                }
            }
        }

        layout()->addStretch(1);
    }
}

SidebarLocator::SidebarLocator(HolonDesktopPrivate *d, QWidget *parent)
:   HBoxWidget(d, parent)
{
    layout()->setSpacing(1);
}

SidebarSelector::SidebarSelector(HolonDesktopPrivate *d)
:   HBoxWidget(d, d->q_ptr)
{
    QPushButton *startButton = new QPushButton(QIcon(":/holon/holoniconlight.svg"), "", this);
    {
        startButton->setFlat(true);
        startButton->setMinimumSize(40, 40);
        startButton->setIconSize({30, 30});
        //startButton->setStyleSheet("qproperty-iconSize: 35px;");
        //startButton->setStyleSheet("border-radius: 0px;");
        startButton->setStyleSheet("background-color: red;");
        layout()->addWidget(startButton);
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

    QPushButton *quitButton = new QPushButton(QIcon(":/holon/quit.svg"), "", this);
    {
        connect(quitButton, &QPushButton::clicked, this, [d]()
        {
            d->q_ptr->deleteLater();
        });

        QShortcut *shortcut = new QShortcut(quitButton);
        {
            shortcut->setKey(QKeySequence("Ctrl+Q"));
            connect(shortcut, &QShortcut::activated, quitButton, [=, this]
            {
                d_ptr->tree->save();
                quitButton->click();
            });
        }

        quitButton->setFlat(true);
        quitButton->setMaximumWidth(20);
        layout()->addWidget(quitButton);
    }
}
