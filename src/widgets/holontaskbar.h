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

#ifndef HOLONTASKBAR_H
#define HOLONTASKBAR_H

#include "holondesktop.h"
#include <QWidget>

class HolonDesktopPrivate;
class HolonSidebarSwitch;
class HolonTaskbarPrivate;
class QBoxLayout;

class HolonTaskbar : public QWidget
{
    Q_OBJECT

    HolonTaskbarPrivate &d;
    std::aligned_storage_t<16, sizeof (ptrdiff_t)> d_storage;

protected:
    void paintEvent(QPaintEvent *) override;

public:
    HolonTaskbar(HolonDesktopPrivate &desktop_d);
    ~HolonTaskbar();

    void addStretch();
    void addWidget(QWidget *widget);
    QBoxLayout *layout() const;
    HolonSidebarSwitch *sidebarSwitch() const;
};

#endif // HOLONTASKBAR_H
