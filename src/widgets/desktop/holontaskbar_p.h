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

#ifndef HOLONTASKBAR_P_H
#define HOLONTASKBAR_P_H

#include "holontaskbar.h"

class HolonTaskbar;
class HolonDesktop;

class HolonTaskbarPrivate
{
public:
    HolonTaskbar *const q_ptr;
    HolonDesktop *const parent;
    HolonTaskbar::Area area;
    int preferedHeight;
    int preferedWidth;

    HolonTaskbarPrivate(HolonTaskbar *q, HolonDesktop *parent);
};

#endif // HOLONTASKBAR_P_H
