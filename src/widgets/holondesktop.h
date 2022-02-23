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

#ifndef HOLONDESKTOP_H
#define HOLONDESKTOP_H

#include "qtholonglobal.h"
#include <QWidget>
#include <QLoaderSettings>

class HolonDesktopPrivate;
class HolonSidebar;
class HolonTask;
class HolonTaskbar;
class HolonWindow;

using QCharList = QList<QChar>;

class Q_HOLON_EXPORT HolonDesktop : public QWidget, public QLoaderSettings
{
    Q_OBJECT

    Q_PROPERTY(QList<QChar> sidebarList READ sidebarList CONSTANT)
    Q_PROPERTY(QStringList sidebarAreaList READ sidebarAreaList CONSTANT)

    friend class HolonDesktopPrivate;
    const QScopedPointer<HolonDesktopPrivate> d_ptr;

protected:
    void closeEvent(QCloseEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    QVariant fromString(const QString &string) const override;
    QString fromVariant(const QVariant &variant) const override;

Q_SIGNALS:
    void sizeChanged(QSize);

public:
    HolonDesktop(QLoaderSettings *settings, QWidget *parent);
    ~HolonDesktop();

    bool addSidebar(HolonSidebar *sidebar);
    void addTask(HolonTask *task);
    void addTaskbar(HolonTaskbar *taskbar);
    void addWindow(HolonWindow *window);
    QString buttonStyleSheet() const;
    QStringList sidebarAreaList() const;
    QCharList sidebarList() const;
    int titleBarHeight() const;
    QString titleBarStyleSheet() const;
};

#endif // HOLONDESKTOP_H
