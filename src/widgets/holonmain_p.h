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

#ifndef HOLONMAIN_P_H
#define HOLONMAIN_P_H

#include <QList>
#include <QMap>
#include <QHash>
#include <QPushButton>

class HolonMain;
class SidebarActivator;
class SidebarLocator;
class HolonWidgetInterface;
class QHBoxLayout;

class HolonMainPrivate
{
public:
    HolonMain *const q_ptr;

    QList<QChar> sidebarList;
    QMap<QChar, bool> sidebarMap;

    QStringList sidebarAreaList;
    QMap<QString, bool> sidebarAreaMap;

    SidebarActivator *sidebarActivator;
    SidebarLocator *sidebarLocator;

    QHash<QString, HolonWidgetInterface*> widgets;


    HolonMainPrivate(HolonMain *q);

    bool mapSidebarArea(QString area);
    bool mapSidebar(QChar sidebar, QString area, Qt::CheckState checkState);
};

class SidebarButton : public QPushButton
{
    Q_OBJECT

    friend class SidebarActivator;

    QChar sidebar;
    QString area;

    SidebarButton(QChar sidebar, QString area, SidebarActivator *parent);
};

class SidebarActivator : public QWidget
{
    Q_OBJECT

    friend class HBoxWidget;
    friend class HolonMainPrivate;

    SidebarActivator(QWidget *parent);

    void insertSidebarButton(int index, QChar sidebar, QString area, Qt::CheckState checkState);
    QHBoxLayout *layout();
    
Q_SIGNALS:
    void sidebarButtonClicked(QChar sidebar, QString area);
};

class HBoxWidget : public QWidget
{
    friend class HolonMain;

    HBoxWidget(HolonMainPrivate *d);

    QHBoxLayout *layout();
};

#endif // HOLONMAIN_P_H
