/****************************************************************************
**
** Copyright (C) 2021, 2022 Sergey Naumov
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

#include <QWidget>
#include <QLoaderSettings>

class HolonSidebarPrivate;
class HolonDesktop;
class HolonTaskbar;

class HolonSidebar : public QWidget, public QLoaderSettings
{
    Q_OBJECT

    QScopedPointer<HolonSidebarPrivate> d_ptr;

public:
    HolonSidebar(QLoaderSettings *settings, HolonDesktop *desktop);
    ~HolonSidebar();

    void addWidget(QWidget *widget);
    QChar sidebar() const;
    QString sidebarArea() const;
};

class HolonSidebarSwitch : public QWidget, public QLoaderSettings
{
    Q_OBJECT

public:
    HolonSidebarSwitch(QLoaderSettings *settings, HolonTaskbar *parent);
};

#endif // HOLONSIDEBAR_H