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

#ifndef HOLONAREASIDEBARS_H
#define HOLONAREASIDEBARS_H

#include "holonareasidebar.h"
#include <QLoaderSettings>

class HolonMainWindow;

class HolonTopSideBar : public HolonAreaSideBar
{
    Q_OBJECT

public:
    HolonTopSideBar(QLoaderSettings *settings, HolonMainWindow *parent);
};


class HolonRightSideBar : public HolonAreaSideBar
{
    Q_OBJECT

public:
    HolonRightSideBar(QLoaderSettings *settings, HolonMainWindow *parent);
};


class HolonBottomSideBar : public HolonAreaSideBar
{
    Q_OBJECT

public:
    HolonBottomSideBar(QLoaderSettings *settings, HolonMainWindow *parent);
};


class HolonLeftSideBar : public HolonAreaSideBar
{
    Q_OBJECT

public:
    HolonLeftSideBar(QLoaderSettings *settings, HolonMainWindow *parent);
};

#endif // HOLONAREASIDEBARS_H
