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

#include "holonmenu.h"
#include "holontaskbar.h"
#include "holondesktop.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QStyleOption>
#include <QPainter>
#include <QShortcut>
#include <QLoaderTree>

class HolonMenuPress : public QObject
{
    HolonMenu *q_ptr;

public:
    HolonMenuPress(HolonMenu *q)
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

class HolonMenuWidget : public QWidget
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
    HolonMenuWidget(HolonMenu *q)
    :   QWidget(q->taskbar()->desktop())
    {
        q->taskbar()->desktop()->installEventFilter(new HolonMenuPress(q));

        setLayout(new QVBoxLayout(this));
        setStyleSheet(q->taskbar()->styleSheet());

        connect(q, &QPushButton::clicked, this, [this, q]()
        {
            if (isVisible())
                hide();
            else
            {
                move(x(), q->taskbar()->desktop()->height() - q->taskbar()->preferedHeight() - height());
                show();
            }
        });

        connect(q->taskbar()->desktop(), &HolonDesktop::sizeChanged, this, [this, q](QSize size)
        {
            move(x(), size.height() - q->taskbar()->preferedHeight() - height());
        });

        QPushButton *quitButton = new QPushButton("Quit", this);
        {
            connect(quitButton, &QPushButton::clicked, this, [q]()
            {
                q->taskbar()->desktop()->deleteLater();
            });

            QShortcut *shortcut = new QShortcut(q->taskbar()->desktop());
            {
                shortcut->setKey(QKeySequence("Ctrl+Q"));
                connect(shortcut, &QShortcut::activated, quitButton, [q, quitButton]
                {
                    q->tree()->save();
                    quitButton->click();
                });
            }

            quitButton->setFlat(true);
            layout()->addWidget(quitButton);
        }
    }
};

HolonMenu::HolonMenu(QLoaderSettings *settings, HolonTaskbar *parent)
:   QPushButton(QIcon(":/holon/holoniconlight.svg"), "", parent),
    QLoaderSettings(settings)
{
    if (!parent)
    {
        emitError("HolonTaskbar not found");
        return;
    } 

    setFlat(true);

    QString stretch = value("stretch").toString();
    if (stretch.contains("before"))
        parent->addStretch();

    parent->addWidget(this);

    if (stretch.contains("after"))
        parent->addStretch();

    int size = parent->preferedHeight();
    if (parent->area() == HolonTaskbar::Top || parent->area() == HolonTaskbar::Bottom)
        setFixedSize(size, size);
    else
        setFixedHeight(size);

    size *= 0.5;
    setIconSize({size, size});

    new HolonMenuWidget(this);
}

HolonTaskbar *HolonMenu::taskbar() const
{
    return qobject_cast<HolonTaskbar*>(parent());
}
