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

#ifndef HOLONSIDEBARDOCK_H
#define HOLONSIDEBARDOCK_H

#include <QDockWidget>

class HolonDesktop;
class HolonDesktopPrivate;
class HolonMainWindow;
class HolonSidebar;
class HolonSidebarDockPrivate;

class HolonSidebarDock : public QDockWidget
{
    Q_OBJECT

    HolonSidebarDockPrivate &d;
    std::aligned_storage_t<48, sizeof (ptrdiff_t)> d_storage;

protected:
    void resizeEvent(QResizeEvent *) override;

public:
    HolonSidebarDock(HolonDesktopPrivate &desktop_d,
                     const QString &name,
                     HolonMainWindow *parent);

    ~HolonSidebarDock();

    void addSidebar(HolonSidebar *sidebar);
    void showTitleBarWidget(bool show);
};

#endif // HOLONSIDEBARDOCK_H
