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

#include "holonnewwindowmenu.h"
#include "holondesktop.h"
#include "holonwindow.h"
#include <QPushButton>
#include <QBoxLayout>
#include <QStyleOption>
#include <QPainter>

class WindowButton : public QPushButton
{
public:
    WindowButton(HolonWindow *window, HolonDesktop *desktop, HolonNewWindowMenu *parent)
    :   QPushButton(parent)
    {
        setIcon(window->icon());
        setFixedHeight(desktop->titleBarHeight());
        setFixedWidth(desktop->newWindowMenuWidth());
        setStyleSheet(desktop->buttonStyleSheet() + "QPushButton { text-align:left; padding-left: 7px; }");
        setText(window->title());

        connect(this, &QPushButton::pressed, this, [window, parent]() { emit parent->triggered(window); });
    }
};

void HolonNewWindowMenu::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

HolonNewWindowMenu::HolonNewWindowMenu(HolonDesktop *desktop, QWidget *parent)
:   QWidget(parent)
{
    int px = desktop->newWindowMenuBorder();
    using Layout = QVBoxLayout;
    Layout *l = new Layout(this);
    setLayout(l);
    {
        layout()->setContentsMargins({px, px, px, px});
        layout()->setSpacing(0);

        for (HolonWindow *w : desktop->windowList())
        {
            if (w->areas().testFlag(HolonWindow::Sidebar))
                l->addWidget(new WindowButton(w, desktop, this), 0, Qt::AlignHCenter);
        }
    }
    setFixedWidth(desktop->newWindowMenuWidth() + px * 2);
    setStyleSheet(desktop->newWindowMenuStyleSheet());
}
