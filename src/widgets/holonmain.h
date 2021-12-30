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

#ifndef HOLONMAIN_H
#define HOLONMAIN_H

#include "qtholonglobal.h"
#include <QMainWindow>
#include <QLoaderSettings>

class HolonSidebar;
class HolonSidebarArea;
class HolonMainPrivate;

class Q_HOLON_EXPORT HolonMain : public QMainWindow, public QLoaderSettings
{
    Q_OBJECT

    Q_PROPERTY(QList<QChar> sidebarList READ sidebarList CONSTANT)
    Q_PROPERTY(QStringList sidebarAreaList READ sidebarAreaList CONSTANT)

    friend class HolonSidebar;
    friend class HolonSidebarArea;

protected:
    const QScopedPointer<HolonMainPrivate> d_ptr;

    void closeEvent(QCloseEvent*) override;

public:
    HolonMain(QLoaderSettings *settings, QWidget *parent);
    ~HolonMain();

    QStringList sidebarAreaList() const;
    QList<QChar> sidebarList() const;
};

#endif // HOLONMAIN_H
