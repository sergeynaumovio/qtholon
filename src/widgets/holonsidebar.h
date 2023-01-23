/****************************************************************************
**
** Copyright (C) 2021-2023 Sergey Naumov
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

#ifndef HOLONSIDEBAR_H
#define HOLONSIDEBAR_H

#include "holonwindowarea.h"
#include "qtholonglobal.h"

class HolonDesktop;
class HolonSidebarPrivate;
class HolonSidebarStack;

class Q_HOLON_EXPORT HolonSidebar : public HolonWindowArea
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonSidebar)

public:
    HolonSidebar(QLoaderSettings *settings, HolonDesktop *desktop);
    HolonSidebar(QLoaderSettings *settings, HolonSidebarStack *stack);
    ~HolonSidebar();

    virtual QIcon icon() const;
    QChar sidebar() const;
    QString sidebarArea() const;
    virtual QString title() const;
};

#endif // HOLONSIDEBAR_H
