// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontaskmenu.h"
#include "holondesktop.h"
#include "holondesktop_p.h"
#include "holontaskbar.h"
#include <QBoxLayout>
#include <QKeyEvent>
#include <QLoaderTree>
#include <QPainter>
#include <QStyleOption>
#include <QShortcut>

#if HOLON_DESKTOP_EMBEDDED
class HolonNewTaskMenuPress : public QObject
{
    HolonNewTaskMenu *q_ptr;

public:
    HolonNewTaskMenuPress(HolonNewTaskMenu *q)
    :   QObject(q),
        q_ptr(q)
    { }

    bool eventFilter(QObject *, QEvent *event) override
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Super_L ||
                keyEvent->key() == Qt::Key_Super_R)
            {
                q_ptr->click();
                return true;
            }
        }

        return false;
    }
};
#endif // HOLON_DESKTOP_EMBEDDED

class HolonNewTaskMenuWidget : public QWidget
{
protected:
    void paintEvent(QPaintEvent *) override
    {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

public:
    HolonNewTaskMenuWidget(HolonDesktopPrivate &desktop_d, HolonTaskMenu *q)
    :   QWidget(desktop_d.q_ptr)
    {
        setLayout(new QVBoxLayout(this));
        setStyleSheet(desktop_d.taskbarStyleSheet());

        #if HOLON_DESKTOP_EMBEDDED
            q->taskbar()->desktop()->installEventFilter(new HolonNewTaskMenuPress(q));
        #endif

        QShortcut *newTaskShortcut = new QShortcut(desktop_d.q_ptr);
        {
            newTaskShortcut->setKey(QKeySequence(q->shortcut()));
            connect(newTaskShortcut, &QShortcut::activated, this, [q]{ q->click(); });
        }
        connect(q, &QPushButton::pressed, this, [this /*, q*/]()
        {
            if (isVisible())
                hide();
            else
            {
                //move(x(), q->taskbar()->desktop()->height() - q->taskbar()->preferedHeight() - height());
                show();
            }
        });

        //connect(q->taskbar()->desktop(), &HolonDesktop::sizeChanged, this, [this, q](QSize size)
        //{
        //    move(x(), size.height() - q->taskbar()->preferedHeight() - height());
        //});

        QPushButton *quitButton = new QPushButton("Quit", this);
        {
            connect(quitButton, &QPushButton::clicked, this, [&desktop_d]()
            {
                desktop_d.q_ptr->deleteLater();
            });

            QShortcut *shortcut = new QShortcut(desktop_d.q_ptr);
            {
                shortcut->setKey(QKeySequence("Ctrl+Q"));
                connect(shortcut, &QShortcut::activated, quitButton, [/*q,*/ quitButton]
                {
                    //q->tree()->save();
                    quitButton->click();
                });
            }

            quitButton->setFlat(true);
            layout()->addWidget(quitButton);
        }
    }
};

HolonTaskMenu::HolonTaskMenu(HolonDesktopPrivate &desktop_d, HolonTaskbar *taskbar)
:   QPushButton(QIcon(":/holon/holoniconlight.svg"), "", taskbar)
{
    setFlat(true);

    int size = desktop_d.taskbarPreferedHeight();
    if (desktop_d.taskbarArea() == HolonDesktopPrivate::Top ||
        desktop_d.taskbarArea() == HolonDesktopPrivate::Bottom)
    {
        setFixedSize(size, size);
    }
    else
        setFixedHeight(size);

    size *= 0.5;
    setIconSize({size, size});

    setStyleSheet(desktop_d.buttonStyleSheet());

    new HolonNewTaskMenuWidget(desktop_d, this);
}
