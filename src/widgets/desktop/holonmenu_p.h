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

#ifndef HOLONMENU_P_H
#define HOLONMENU_P_H

#include <QWidget>

class HolonMenu;
class HolonTaskbar;
class HolonMenuWidget;
class HolonDesktop;

class HolonMenuPrivate
{
public:
    HolonMenu *const q_ptr;
    HolonTaskbar *const taskbar;
    HolonMenuWidget *menu;

    HolonMenuPrivate(HolonMenu *q, HolonTaskbar *parent);
};

class HolonMenuWidget : public QWidget
{
    Q_OBJECT

    HolonMenuPrivate *d_ptr;

protected:
    void paintEvent(QPaintEvent *) override;

public:
    HolonMenuWidget(HolonMenuPrivate *d, HolonDesktop *parent);
};

#endif // HOLONMENU_P_H
