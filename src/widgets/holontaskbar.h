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

#ifndef HOLONTASKBAR_H
#define HOLONTASKBAR_H

#include "qtholonglobal.h"
#include <QWidget>
#include <QLoaderSettings>

class HolonTaskbarPrivate;
class HolonDesktop;
class QBoxLayout;

class Q_HOLON_EXPORT HolonTaskbar : public QWidget, public QLoaderSettings
{
    Q_OBJECT

    Q_PROPERTY(HolonTaskbar::Area area READ area CONSTANT)
    Q_PROPERTY(int preferedHeight READ preferedHeight CONSTANT)
    Q_PROPERTY(int preferedWidth READ preferedWidth CONSTANT)

    const QScopedPointer<HolonTaskbarPrivate> d_ptr;

protected:
    void paintEvent(QPaintEvent *) override;

public:
    enum Area
    {
        Left,
        Right,
        Top,
        Bottom
    };
    Q_ENUM(Area)

    HolonTaskbar(QLoaderSettings *settings, HolonDesktop *parent);
    ~HolonTaskbar();

    void addStretch();
    void addWidget(QWidget *widget);
    HolonTaskbar::Area area() const;
    HolonDesktop *desktop() const;
    QBoxLayout *layout() const;
    int preferedHeight() const;
    int preferedWidth() const;
};

#endif // HOLONTASKBAR_H
