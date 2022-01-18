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

#ifndef HOLONDESKTOP_P_H
#define HOLONDESKTOP_P_H

#include <QList>
#include <QMap>
#include <QSet>
#include <QHash>
#include <QPushButton>

class HolonDesktop;
class HolonTaskbar;
class QLoaderTree;
class HolonSidebarArea;
class HolonSidebar;
class SidebarButton;
class SidebarActivator;
class SidebarLocator;
class HolonWidgetInterface;
class QHBoxLayout;

struct SidebarRelatedObjects
{
    HolonSidebarArea *area;
    HolonSidebar *sidebar;
    SidebarButton *button;
};

class HolonDesktopPrivate
{
public:
    HolonDesktop *const q_ptr;
    QLoaderTree *tree;

    struct
    {
        HolonTaskbar *taskbar{};
        QWidget *workspace{};

    } desktop;

    QList<QChar> sidebarList;

    QMap<QChar, SidebarRelatedObjects> sidebarRelatedObjects;

    QStringList sidebarAreaList;
    QMap<QString, HolonSidebarArea*> sidebarAreas;

    SidebarActivator *sidebarActivator{};
    SidebarLocator *sidebarLocator{};

    QHash<QString, HolonWidgetInterface*> widgets;


    HolonDesktopPrivate(HolonDesktop *q);

    bool mapSidebarArea(QString area, HolonSidebarArea *q);
    bool mapSidebar(QPair<QChar, HolonSidebar*> sidebar,
                    QPair<QString, HolonSidebarArea*> area, Qt::CheckState checkState);
    bool setTaskbar(HolonTaskbar *taskbar);
    bool setWorkspace(QWidget *widget);
};

class HBoxWidget : public QWidget
{
    Q_OBJECT

    using Layout = QHBoxLayout;

protected:
    HolonDesktopPrivate *d_ptr;

    HBoxWidget(HolonDesktopPrivate *d, QWidget *parent);

    Layout *layout();
};

class SidebarButton : public QPushButton
{
    Q_OBJECT

    friend class SidebarActivator;
    friend class SidebarLocator;
    friend class HolonSidebarArea;

    QChar sidebar;
    QString area;

    SidebarButton(QChar sidebar, QString area, SidebarActivator *parent);
};

class SidebarActivator : public HBoxWidget
{
    Q_OBJECT

    friend class SidebarSelector;
    SidebarActivator(HolonDesktopPrivate *d, QWidget *parent);

    friend class HolonDesktopPrivate;
    void insertSidebarButton(int index, QChar sidebar, QString area, Qt::CheckState checkState);

Q_SIGNALS:
    void sidebarButtonClicked(QChar sidebar, QString area);
};

class SidebarLocator : public HBoxWidget
{
    Q_OBJECT

    bool once{};

    void showEvent(QShowEvent*) override;

    friend class SidebarSelector;
    SidebarLocator(HolonDesktopPrivate *d, QWidget *parent);
};

class SidebarSelector : public HBoxWidget
{
    Q_OBJECT

    friend class HolonDesktop;
    SidebarSelector(HolonDesktopPrivate *d);
};

#endif // HOLONDESKTOP_P_H
