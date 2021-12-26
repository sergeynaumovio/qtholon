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
#include <QShortcut>
#include <QStackedWidget>

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

class SidebarActivator : public QWidget
{
public:
    SidebarActivator(HolonMain *mainWindow, QWidget *parent)
    :   QWidget(parent)
    {
        setLayout(new QHBoxLayout(this));
        layout()->setContentsMargins({});

        connect(mainWindow, &HolonMain::sidebarAdded, this, [=, this](QString sidebar, QString area)
        {
            QPushButton *button = new QPushButton(sidebar, this);
            {
                button->setCheckable(true);
                button->setProperty("sidebar", sidebar);
                button->setProperty("area", area);
                button->setFlat(true);

                connect(button, &QPushButton::clicked, button, [mainWindow, button]()
                {
                    emit mainWindow->sidebarToggled(button->property("sidebar").toString(),
                                                    button->property("area").toString());
                });

                connect(mainWindow, &HolonMain::sidebarActivated, button, [button](QString sidebar)
                {
                    if(sidebar == button->property("sidebar").toString())
                         button->setChecked(true);
                });

                connect(mainWindow, &HolonMain::sidebarToggled, button, [button](QString sidebar, QString area)
                {
                    if (area == button->property("area").toString() &&
                        sidebar != button->property("sidebar").toString())
                    {
                        button->setChecked(false);
                    }
                });

                QShortcut *shortcut = new QShortcut(button);
                {
                    shortcut->setKey("Alt+" + sidebar);
                    connect(shortcut, &QShortcut::activated, button, [button]{ button->click(); });
                }

                layout()->addWidget(button);
            }
        });
    }
};

class SidebarLocator : public QWidget
{
public:
    SidebarLocator(HolonMain*, QWidget *parent)
    :   QWidget(parent)
    {
        setLayout(new QHBoxLayout(this));
        layout()->setContentsMargins({});
    }
};

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

            int activatorIndex = sidebarSelector->addWidget(new SidebarActivator(mainWindow, this));
            int locatorIndex = sidebarSelector->addWidget(new SidebarLocator(mainWindow, this));

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
            connect(exit, &QPushButton::clicked, this, [mainWindow]()
            {
                mainWindow->deleteLater();
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

