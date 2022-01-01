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

#ifndef HOLONMAIN_P_H
#define HOLONMAIN_P_H

#include <QList>
#include <QMap>
#include <QSet>
#include <QHash>
#include <QPushButton>

class HolonMain;
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

class HolonMainPrivate
{
public:
    HolonMain *const q_ptr;

    QList<QChar> sidebarList;

    QMap<QChar, SidebarRelatedObjects> sidebarMap;

    QStringList sidebarAreaList;
    QMap<QString, HolonSidebarArea*> sidebarAreaMap;

    SidebarActivator *sidebarActivator;
    SidebarLocator *sidebarLocator;

    QHash<QString, HolonWidgetInterface*> widgets;


    HolonMainPrivate(HolonMain *q);

    bool mapSidebarArea(QString area, HolonSidebarArea *q);
    bool mapSidebar(QPair<QChar, HolonSidebar*> sidebar,
                    QPair<QString, HolonSidebarArea*> area, Qt::CheckState checkState);
};

class HBoxWidget : public QWidget
{
    Q_OBJECT

    using Layout = QHBoxLayout;

protected:
    HolonMainPrivate *d_ptr;

    HBoxWidget(HolonMainPrivate *d, QWidget *parent);

    Layout *layout();
};

class SidebarButton : public QPushButton
{
    Q_OBJECT

    friend class SidebarActivator;
    friend class SidebarLocator;

    QChar sidebar;
    QString area;

    SidebarButton(QChar sidebar, QString area, SidebarActivator *parent);
};

class SidebarActivator : public HBoxWidget
{
    Q_OBJECT

    friend class SidebarSelector;
    SidebarActivator(HolonMainPrivate *d, QWidget *parent);

    friend class HolonMainPrivate;
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
    SidebarLocator(HolonMainPrivate *d, QWidget *parent);
};

class SidebarSelector : public HBoxWidget
{
    Q_OBJECT

    friend class HolonMain;
    SidebarSelector(HolonMainPrivate *d);
};

#endif // HOLONMAIN_P_H
