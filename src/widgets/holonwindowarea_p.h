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

#ifndef HOLONWINDOWAREA_P_H
#define HOLONWINDOWAREA_P_H

#include "holonwindowarea.h"

class HolonDesktop;
class QMainWindow;
class QDockWidget;

class HolonWindowAreaPrivate
{
public:
    HolonWindowArea *q_ptr;
    HolonDesktop *const desktop;
    QMainWindow *const mainWindow;
    QDockWidget *const defaultDock;
    QList<QDockWidget*> dockList;
    bool maximized{};

    HolonWindowAreaPrivate(HolonDesktop *desktop);
    virtual ~HolonWindowAreaPrivate();

    void addWindow(HolonWindow *window);
    void maximizeWindow(QDockWidget *dock);
    void closeWindow(QDockWidget *dock, HolonWindow *window);

};

#endif // HOLONWINDOWAREA_P_H
