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

#ifndef HOLONWINDOWAREA_H
#define HOLONWINDOWAREA_H

#include <QLoaderSettings>
#include <QWidget>

class HolonDesktop;
class HolonWindow;
class HolonWindowAreaPrivate;
class QStackedWidget;

class HolonWindowArea : public QWidget, public QLoaderSettings
{
    Q_OBJECT

protected:
    const QScopedPointer<HolonWindowAreaPrivate> d_ptr;

    HolonWindowArea(HolonWindowAreaPrivate &d,
                    QLoaderSettings *settings);

public:
    HolonWindowArea(QLoaderSettings *settings,
                    HolonDesktop *desktop,
                    QStackedWidget *parent);

    ~HolonWindowArea();

    void addWindow(HolonWindow *window);
};

#endif // HOLONWINDOWAREA_H
