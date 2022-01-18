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
#include <QMainWindow>
#include <QLoaderSettings>

class HolonDesktopPrivate;
class HolonTaskbar;
class HolonWorkspace;

class Q_HOLON_EXPORT HolonDesktop : public QMainWindow, public QLoaderSettings
{
    Q_OBJECT

    Q_PROPERTY(QList<QChar> sidebarList READ sidebarList CONSTANT)
    Q_PROPERTY(QStringList sidebarAreaList READ sidebarAreaList CONSTANT)

    friend class HolonDesktopPrivate;
    friend class HolonSidebar;
    friend class HolonSidebarArea;

protected:
    const QScopedPointer<HolonDesktopPrivate> d_ptr;

    void closeEvent(QCloseEvent *) override;
    void resizeEvent(QResizeEvent *) override;

Q_SIGNALS:
    void sizeChanged(QSize);

public:
    HolonDesktop(QLoaderSettings *settings, QWidget *parent);
    ~HolonDesktop();

    void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget) = delete;
    void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget, Qt::Orientation orientation) = delete;
    void addToolBar(Qt::ToolBarArea area, QToolBar *toolbar) = delete;
    void addToolBar(QToolBar *toolbar) = delete;
    QToolBar *addToolBar(const QString &title) = delete;
    void addToolBarBreak(Qt::ToolBarArea area = Qt::TopToolBarArea) = delete;
    void setCentralWidget(QWidget *widget) = delete;
    void setCorner(Qt::Corner corner, Qt::DockWidgetArea area) = delete;
    void setMenuBar(QMenuBar *menuBar) = delete;
    void setStatusBar(QStatusBar *statusbar) = delete;
    QWidget *statusBar() = delete;

    QStringList sidebarAreaList() const;
    QList<QChar> sidebarList() const;

    bool setTaskbar(HolonTaskbar *taskbar);
    bool setWorkspace(QWidget *widget);
    HolonTaskbar *taskbar() const;
    QWidget *workspace() const;
};


#endif // HOLONDESKTOP_H
