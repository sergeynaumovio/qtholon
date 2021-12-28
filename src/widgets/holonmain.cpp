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
#include <QRegularExpression>

class HolonWidgetInterface;
class SidebarActivator;
class SidebarLocator;

class HolonMainPrivate
{
public:
    HolonMain *const q_ptr;

    QList<QChar> sidebarList;
    QMap<QChar, bool> sidebarMap;

    QStringList sidebarAreaList;
    QMap<QString, bool> sidebarAreaMap;

    SidebarActivator *sidebarActivator;
    SidebarLocator *sidebarLocator;

    QHash<QString, HolonWidgetInterface*> widgets;

    HolonMainPrivate(HolonMain *q);
};

HolonMainPrivate::HolonMainPrivate(HolonMain *q)
:   q_ptr(q)
{ }

class SidebarActivator : public QWidget
{
public:
    SidebarActivator(QWidget *parent)
    :   QWidget(parent)
    {
        setLayout(new QHBoxLayout(this));
        layout()->setContentsMargins({});
    }
};

class SidebarLocator : public QWidget
{
    bool once{};

    void set()
    {

    }

public:
    SidebarLocator(HolonMain *mainWindow, QWidget *parent)
    :   QWidget(parent)
    {
        setLayout(new QHBoxLayout(this));
        layout()->setContentsMargins({});

        connect(mainWindow, &HolonMain::sidebarSelectorWidgetChanged, this, [=, this](QWidget *widget)
        {
            if (!once && widget == this)
            {
                once = true;

                set();
            }
        });
    }
};

class HBoxWidget : public QWidget
{
    QHBoxLayout *layout()
    {
        return static_cast<QHBoxLayout*>(QWidget::layout());
    }

public:
    HBoxWidget(HolonMainPrivate *d_ptr)
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

            d_ptr->sidebarActivator = new SidebarActivator(this);
            d_ptr->sidebarLocator = new SidebarLocator(d_ptr->q_ptr, this);
            int activatorIndex = sidebarSelector->addWidget(d_ptr->sidebarActivator);
            int locatorIndex = sidebarSelector->addWidget(d_ptr->sidebarLocator);

            connect(modeButton, &QPushButton::toggled, sidebarSelector, [=](bool checked)
            {
                if (checked)
                    sidebarSelector->setCurrentIndex(locatorIndex);
                else
                    sidebarSelector->setCurrentIndex(activatorIndex);

                emit d_ptr->q_ptr->sidebarSelectorWidgetChanged(sidebarSelector->currentWidget());
            });
        }

        QPushButton *exit = new QPushButton(QIcon(":/holon/exit.svg"), "", this);
        {
            connect(exit, &QPushButton::clicked, this, [d_ptr]()
            {
                d_ptr->q_ptr->deleteLater();
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

    if (contains("sidebarList"))
    {
        QRegularExpression whiteSpace("\\s+");
        const QStringList sidebarList = value("sidebarList").toString().remove(whiteSpace).split(',');
        for (const QString &s : sidebarList)
        {
            if (s.size() != 1)
            {
                setObjectError("sidebarList item is not a char");
                return;
            }
            d_ptr->sidebarList.append(s.at(0));
            d_ptr->sidebarMap.insert(s.at(0), false);
        }
    }
    else
    {
        setObjectError("sidebarList property is not set");
        return;
    }

    if (contains("sidebarAreaList"))
    {
        QRegularExpression whiteSpace("\\s+");
        d_ptr->sidebarAreaList = value("sidebarAreaList").toString().remove(whiteSpace).split(',');
    }
    else
    {
        setObjectError("sidebarAreaList property is not set");
        return;
    }


    statusBar()->addWidget(new HBoxWidget(d_ptr.data()), 1);

    setStyleSheet("QStatusBar { background-color : rgb(64, 66, 68) }"
                  "QStatusBar::item { border: 0px }" ) ;
}

HolonMain::~HolonMain()
{ }

bool HolonMain::addSidebarButton(QChar sidebar)
{
    if (d_ptr->sidebarMap.contains(sidebar))
    {
        if (d_ptr->sidebarMap.value(sidebar))
            return false;
        else
            d_ptr->sidebarMap[sidebar] = true;
    }
    else
        return false;

    QPushButton *button = new QPushButton(sidebar, d_ptr->sidebarActivator);
    {
        button->setCheckable(true);
        button->setFlat(true);

        d_ptr->sidebarActivator->layout()->addWidget(button);

        connect(button, &QPushButton::clicked, button, [=, this]()
        {
            emit sidebarToggled(button->property("sidebar").toString(),
                                button->property("area").toString());
        });

        connect(this, &HolonMain::sidebarActivated, button, [button](QString sidebar)
        {
            if (sidebar == button->property("sidebar").toString())
                 button->setChecked(true);
        });

        connect(this, &HolonMain::sidebarToggled, button, [button](QString sidebar, QString area)
        {
            if (area == button->property("area").toString() &&
                sidebar != button->property("sidebar").toString())
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

    return true;
}

QStringList HolonMain::sidebarAreaList() const
{
    return d_ptr->sidebarAreaList;
}

QList<QChar> HolonMain::sidebarList() const
{
    return d_ptr->sidebarList;
}
