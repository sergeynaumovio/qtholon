/****************************************************************************
**
** Copyright (C) 2023 Sergey Naumov
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

#ifndef HOLONDESKTOP_P_H
#define HOLONDESKTOP_P_H

#include "holontaskbar.h"
#include <QRegularExpression>
#include <QStringList>

class HolonDesktop;
class HolonMainWindow;
class HolonSidebar;
class HolonTaskbar;
class HolonWindow;
class QWidget;

class HolonDesktopPrivate
{
    QWidget *screen;
    QWidget *top;
    QWidget *middle;
    QWidget *bottom;
    QWidget *left;
    QWidget *right;

    void addWidget(QWidget *widget, QWidget *parent);
    void setHBoxLayout(QWidget *&widget, const char *name, QWidget *parent);
    void setMainWindow(HolonMainWindow *&widget, QWidget *parent);
    void setTaskbar();
    void setVBoxLayout();
    void setVBoxLayout(QWidget *&widget, const char *name, QWidget *parent);
    HolonTaskbar::Area taskbarArea() const;
    int taskbarPreferedHeight() const;
    int taskbarPreferedWidth() const;
    QString taskbarStyleSheet() const;

public:
    HolonDesktop *const q_ptr;
    QString error;
    const QString buttonStyleSheet;
    const QString menuStyleSheet;
    const QRegularExpression borderWidth{"^QWidget\\s*{[^}]*border:[^};]*(?<px>\\d+)px[^}]*}$"};
    const int menuBorder;
    const int menuWidth;
    QList<HolonSidebar*> sidebarList;
    const QString sidebarMoveShortcut;
    const int titleBarHeight;
    const QString titleBarStyleSheet;
    QList<HolonWindow *> windowList;
    HolonMainWindow *mainWindow;
    int skipMainWindowSaveState{2};
    HolonTaskbar *taskbar;

    HolonDesktopPrivate(HolonDesktop *q);

    void addSidebar(HolonSidebar *sidebar);
    void setLayout();
};

#endif // HOLONDESKTOP_P_H
