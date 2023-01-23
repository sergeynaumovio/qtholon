/****************************************************************************
**
** Copyright (C) 2022-2023 Sergey Naumov
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

#ifndef HOLONWINDOWMENU_H
#define HOLONWINDOWMENU_H

#include "qtholonglobal.h"
#include <QWidget>

class HolonDesktop;
class HolonWindow;

class Q_HOLON_EXPORT HolonWindowMenu : public QWidget
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent *) override;

Q_SIGNALS:
    void triggered(HolonWindow *window);

public:
    HolonWindowMenu(HolonDesktop *desktop, QWidget *parent);
};

#endif // HOLONWINDOWMENU_H
