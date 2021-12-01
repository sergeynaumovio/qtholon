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

#ifndef HOLONSTATUSBAR_H
#define HOLONSTATUSBAR_H

#include "qtholonglobal.h"
#include <QStatusBar>
#include <QLoaderSettings>

class HolonMainWindow;

class Q_HOLON_EXPORT HolonStatusBar : public QStatusBar, public QLoaderSettings
{
    Q_OBJECT
    Q_INTERFACES(QLoaderSettings)

public:
    HolonStatusBar(QLoaderSettings *settings, HolonMainWindow *parent);

    HolonMainWindow *mainWindow() const;
};

#endif // HOLONSTATUSBAR_H
