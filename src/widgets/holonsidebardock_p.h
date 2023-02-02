/****************************************************************************
**
** Copyright (C) 2023 Sergey Naumov
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

#ifndef HOLONSIDEBARDOCK_P_H
#define HOLONSIDEBARDOCK_P_H

#include <stddef.h>
#include <type_traits>

class HolonDesktopPrivate;
class HolonMainWindow;
class HolonSidebar;
class HolonSidebarDock;
class HolonSidebarDockPrivateData;
class QStackedWidget;
class QString;

class HolonSidebarDockPrivate
{
    HolonSidebarDockPrivateData &d;
    std::aligned_storage_t<56, sizeof (ptrdiff_t)> d_storage;

public:
    HolonSidebarDockPrivate(HolonDesktopPrivate &desktop_d,
                            HolonSidebarDock *q,
                            const QString &name,
                            HolonMainWindow *parent);

    HolonSidebar *currentSidebar() const;
    HolonDesktopPrivate &desktop_d() const;
    void showTitleBarWidget(bool show) const;
    QStackedWidget *stackedWidget() const;

};

#endif // HOLONSIDEBARDOCK_P_H
