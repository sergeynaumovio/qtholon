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

#ifndef HOLONSIDEBARSWITCH_H
#define HOLONSIDEBARSWITCH_H

#include <QWidget>

class HolonDesktopPrivate;
class HolonSidebar;
class HolonSidebarSwitchPrivate;
class HolonTaskbar;

class HolonSidebarSwitch : public QWidget
{
    Q_OBJECT

    HolonSidebarSwitchPrivate &d;
    std::aligned_storage_t<8, sizeof (ptrdiff_t)> d_storage;

public:
    HolonSidebarSwitch(HolonDesktopPrivate &desktop_d, HolonTaskbar *parent);

    void addSidebar(HolonSidebar *sidebar);
};

#endif // HOLONSIDEBARSWITCH_H
