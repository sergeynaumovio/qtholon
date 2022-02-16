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

#ifndef HOLONWINDOW_H
#define HOLONWINDOW_H

#include "qtholonglobal.h"
#include <QLoaderSettings>

class Q_HOLON_EXPORT HolonWindow : public QObject, public QLoaderSettings
{
    Q_OBJECT

public:
    HolonWindow(QLoaderSettings *settings, QObject *parent);

    virtual QIcon icon() const;
    virtual QString title() const;
    virtual QWidget *toolbar() const;
    virtual QWidget *widget() const;
};

#endif // HOLONWINDOW_H
