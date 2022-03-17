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

#ifndef HOLONOPENTASKSWINDOW_H
#define HOLONOPENTASKSWINDOW_H

#include "holonwindow.h"
#include <QWidget>
#include <QLoaderCopyInterface>

class HolonOpenTasksWindowPrivate;
class HolonDesktop;
class HolonSidebar;

class HolonOpenTasksWindow : public HolonWindow, public QLoaderCopyInterface
{
    Q_OBJECT
    Q_INTERFACES(QLoaderCopyInterface)

    QScopedPointer<HolonOpenTasksWindowPrivate> d_ptr;

public:
    HolonOpenTasksWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonOpenTasksWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonOpenTasksWindow();

    HolonWindow::Areas areas() const override;
    bool copy(const QStringList &to) override;
    QIcon icon() const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget() const override;

};

class HolonOpenTasksToolbar : public QWidget
{

};

class HolonOpenTasksWidget : public QWidget
{

};

#endif // HOLONOPENTASKSWINDOW_H
