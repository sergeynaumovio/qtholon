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

#ifndef HOLONWIDGETINTERFACE_H
#define HOLONWIDGETINTERFACE_H

#include "qtholonglobal.h"
#include <QLoaderSettings>

class HolonMain;
class HolonTiled;
class QWidget;

class Q_HOLON_EXPORT HolonWidgetInterface : public QObject, public QLoaderSettings
{
    Q_OBJECT

protected:
    HolonWidgetInterface(QLoaderSettings *settings, HolonMain *parent);
    HolonWidgetInterface(QLoaderSettings *settings, HolonTiled *parent);
    HolonMain *mainWindow() const;

public:
    virtual QWidget *widget() = 0;
};

#endif // HOLONWIDGETINTERFACE_H
