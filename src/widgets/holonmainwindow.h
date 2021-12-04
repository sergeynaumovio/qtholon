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

#ifndef HOLONMAINWINDOW_H
#define HOLONMAINWINDOW_H

#include "qtholonglobal.h"
#include <QMainWindow>
#include <QLoaderSettings>

class HolonMainWindowPrivate;
class HolonAreaSideBar;
class HolonSideWidget;

class Q_HOLON_EXPORT HolonMainWindow : public QMainWindow, public QLoaderSettings
{
    Q_OBJECT

protected:
    const QScopedPointer<HolonMainWindowPrivate> d_ptr;

    void closeEvent(QCloseEvent*) override;

public:
    HolonMainWindow(QLoaderSettings *settings, QWidget *parent);
    ~HolonMainWindow();

    void addSideBar(HolonAreaSideBar *area);
    void addSideWidget(HolonSideWidget *widget);
};

#endif // HOLONMAINWINDOW_H
