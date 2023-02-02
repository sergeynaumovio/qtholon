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

#include "holondesktop_p.h"
#include "holondesktop.h"
#include "holonmainwindow.h"
#include "holonsidebar.h"
#include "holonsidebar_p.h"
#include "holonsidebardock.h"
#include "holonsidebardock_p.h"
#include "holontaskbar.h"
#include "holonsidebarswitch.h"
#include <QLayout>
#include <QLoaderTree>
#include <QShortcut>
#include <QStringList>
#include <QRegularExpression>

class HolonDesktopPrivateData
{
public:
    HolonDesktopPrivate &desktop_d;
    HolonDesktop *const q_ptr;
    const QString buttonStyleSheet;
    const QString menuStyleSheet;
    const QRegularExpression borderWidth{"^QWidget\\s*{[^}]*border:[^};]*(?<px>\\d+)px[^}]*}$"};
    const int menuBorder;
    const int menuWidth;
    const QString sidebarMoveShortcut;
    const int sidebarSwitchButtonWidth;
    const HolonDesktopPrivate::TaskbarArea taskbarArea;
    const int taskbarPreferedHeight;
    const int taskbarPreferedWidth;
    const QString taskbarStyleSheet;
    const int titleBarHeight;
    const QString titleBarStyleSheet;

    QWidget *screen;
    QWidget *top;
    QWidget *middle;
    QWidget *bottom;
    QWidget *left;
    QWidget *right;

    QList<HolonWindow *> windowList;
    HolonMainWindow *externalMainWindow;
    HolonMainWindow *internalMainWindow;
    int skipExternalMainWindowSaveState{2};
    HolonTaskbar *taskbar;
    struct
    {
        QMap<HolonSidebarDock *, HolonMainWindow *> mainWindowByDock;
        QSet<HolonSidebarDock *> docks;
        QMap<HolonSidebar *, HolonSidebarDock *> dockBySidebar;
        QMap<HolonSidebarDock *, int> dockWidth;

        struct
        {
            QList<QDockWidget *> dockList;
            QList<int> widthList;

        } visible;

    } sidebars;

    HolonDesktopPrivateData(HolonDesktopPrivate &d, HolonDesktop *q);
    ~HolonDesktopPrivateData() { }

    void addSidebar(HolonSidebar *sidebar);
    void addWidget(QWidget *widget, QWidget *parent);
    void resizeEvent();
    void setHBoxLayout(QWidget *&widget, const char *name, QWidget *parent);
    void setLayout();
    void setMainWindow(HolonMainWindow *&widget, const char *name, QWidget *parent);
    void setTaskbar();
    void setVBoxLayout();
    void setVBoxLayout(QWidget *&widget, const char *name, QWidget *parent);
};

HolonDesktopPrivateData::HolonDesktopPrivateData(HolonDesktopPrivate &d, HolonDesktop *q)
:   desktop_d(d),
    q_ptr(q),
    buttonStyleSheet(q_ptr->value("buttonStyleSheet").toString()),
    menuStyleSheet(q_ptr->value("menuStyleSheet").toString()),
    menuBorder([this]()
    {
        QRegularExpressionMatch match = borderWidth.match(menuStyleSheet);
        if (match.hasMatch())
            return match.captured("px").toInt();

        return 1;
    }()),
    menuWidth(q_ptr->value("menuWidth", 200).toInt()),
    sidebarMoveShortcut(q_ptr->value("sidebarMoveShortcut").toString()),
    sidebarSwitchButtonWidth(q_ptr->value("sidebarSwitchButtonWidth", 80).toInt()),
    taskbarArea([this]()
    {
        QString string = q_ptr->value("taskbarArea").toString();
        if (string == "left")
            return HolonDesktopPrivate::TaskbarArea::Left;

        if (string == "right")
            return HolonDesktopPrivate::TaskbarArea::Right;

        if (string == "top")
            return HolonDesktopPrivate::TaskbarArea::Top;

        return HolonDesktopPrivate::TaskbarArea::Bottom;
    }()),
    taskbarPreferedHeight(q_ptr->value("taskbarPreferedHeight", 40).toInt()),
    taskbarPreferedWidth(q_ptr->value("taskbarPreferedWidth", 50).toInt()),
    taskbarStyleSheet(q_ptr->value("taskbarStyleSheet").toString()),
    titleBarHeight(q_ptr->value("titleBarHeight", 10).toInt()),
    titleBarStyleSheet(q_ptr ->value("titleBarStyleSheet").toString())
{ }

void HolonDesktopPrivateData::addSidebar(HolonSidebar *sidebar)
{
    HolonSidebarDock *sidebarDock;

    if (sidebar->value("layout", "external").toString() == "external")
    {
        sidebarDock = externalMainWindow->addSidebar(sidebar);
        sidebars.mainWindowByDock.insert(sidebarDock, externalMainWindow);
    }
    else
    {
        sidebarDock = internalMainWindow->addSidebar(sidebar);
        sidebars.mainWindowByDock.insert(sidebarDock, internalMainWindow);
    }

    sidebars.docks.insert(sidebarDock);

    sidebars.dockBySidebar.insert(sidebar, sidebarDock);

    sidebars.visible.dockList.reserve(sidebars.dockWidth.size());
    sidebars.visible.widthList.reserve(sidebars.dockWidth.size());

    externalMainWindow->restoreState(q_ptr->value("externalMainWindowState").toByteArray());
    internalMainWindow->restoreState(q_ptr->value("internalMainWindowState").toByteArray());
    taskbar->sidebarSwitch()->addSidebar(sidebar);
}

void HolonDesktopPrivateData::addWidget(QWidget *widget, QWidget *parent)
{
    parent->layout()->addWidget(widget);
}

void HolonDesktopPrivateData::resizeEvent()
{
    if (skipExternalMainWindowSaveState)
        skipExternalMainWindowSaveState--;
}

void HolonDesktopPrivateData::setHBoxLayout(QWidget *&widget, const char *name, QWidget *parent)
{
    widget = new QWidget(parent);
    widget->setObjectName(name);
    widget->setLayout(new QHBoxLayout(widget));
    widget->layout()->setContentsMargins({});
    widget->layout()->setSpacing(0);
    parent->layout()->addWidget(widget);
}

void HolonDesktopPrivateData::setLayout()
{
    setVBoxLayout();
    setVBoxLayout(screen, "Screen", q_ptr);
    {
        setVBoxLayout(top, "ScreenTop", screen);
        setHBoxLayout(middle, "ScreenMiddle", screen);
        {
            setHBoxLayout(left, "ScreenLeft", middle);
            setMainWindow(externalMainWindow, "ScreenCenterExternal", middle);
            setMainWindow(internalMainWindow, "ScreenCenterInternal", externalMainWindow);
            setHBoxLayout(right, "ScreenRight", middle);
        }
        setVBoxLayout(bottom, "ScreenBottom", screen);
    }
    setTaskbar();
}

void HolonDesktopPrivateData::setMainWindow(HolonMainWindow *&widget, const char *name, QWidget *parent)
{
    widget = new HolonMainWindow(desktop_d, parent);
    widget->setObjectName(name);
}

void HolonDesktopPrivateData::setTaskbar()
{
    QWidget *parent{};

    switch (taskbarArea) {
    case HolonDesktopPrivate::Top: parent = top; break;
    case HolonDesktopPrivate::Bottom: parent = bottom; break;
    case HolonDesktopPrivate::Left: parent = left; break;
    case HolonDesktopPrivate::Right: parent = right; break;
    }

    taskbar = new HolonTaskbar(desktop_d);

    addWidget(taskbar, parent);
}

void HolonDesktopPrivateData::setVBoxLayout()
{
    q_ptr->setLayout(new QVBoxLayout(q_ptr));
    q_ptr->layout()->setContentsMargins({});
    q_ptr->layout()->setSpacing(0);
}

void HolonDesktopPrivateData::setVBoxLayout(QWidget *&widget, const char *name, QWidget *parent)
{
    widget = new QWidget(parent);
    widget->setObjectName(name);
    widget->setLayout(new QVBoxLayout(widget));
    widget->layout()->setContentsMargins({});
    widget->layout()->setSpacing(0);
    parent->layout()->addWidget(widget);
}

HolonDesktopPrivate::HolonDesktopPrivate(HolonDesktop *q)
:   d(*new (&d_storage) HolonDesktopPrivateData(*this, q)),
    q_ptr(q)
{
    static_assert (sizeof (d_storage) == sizeof (HolonDesktopPrivateData));
    static_assert (sizeof (ptrdiff_t) == alignof (HolonDesktopPrivateData));

    if (q->contains("saveShortcut") == QLoaderSettings::Value)
    {
        QShortcut *shortcut = new QShortcut(q);
        {
            shortcut->setKey(QKeySequence(q->value("saveShortcut").toString()));
            QObject::connect(shortcut, &QShortcut::activated, q, [q] { q->tree()->save(); });
        }
    }
}

void HolonDesktopPrivate::addSidebar(HolonSidebar *sidebar)
{
    d.addSidebar(sidebar);
}

void HolonDesktopPrivate::addWindow(HolonWindow *window)
{
    d.windowList.append(window);
}
void HolonDesktopPrivate::resizeEvent(QResizeEvent *)
{
    if (d.skipExternalMainWindowSaveState)
        d.skipExternalMainWindowSaveState--;
}

void HolonDesktopPrivate::setLayout()
{
    d.setLayout();
}

HolonDesktopPrivate::~HolonDesktopPrivate()
{
    d.~HolonDesktopPrivateData();
}

QString HolonDesktopPrivate::buttonStyleSheet() const
{
    return d.buttonStyleSheet;
}

int HolonDesktopPrivate::menuBorder() const
{
    return d.menuBorder;
}

QString HolonDesktopPrivate::menuStyleSheet() const
{
    return d.menuStyleSheet;
}

int HolonDesktopPrivate::menuWidth() const
{
    return d.menuWidth;
}

const QSet<HolonSidebarDock *> &HolonDesktopPrivate::sidebarDocks() const
{
    return d.sidebars.docks;
}

int HolonDesktopPrivate::titleBarHeight() const
{
    return d.titleBarHeight;
}

QString HolonDesktopPrivate::titleBarStyleSheet() const
{
    return d.titleBarStyleSheet;
}

QString HolonDesktopPrivate::sidebarMoveShortcut() const
{
    return d.sidebarMoveShortcut;
}

int HolonDesktopPrivate::sidebarSwitchButtonWidth() const
{
    return d.sidebarSwitchButtonWidth;
}

HolonTaskbar *HolonDesktopPrivate::taskbar() const
{
    return d.taskbar;
}

HolonDesktopPrivate::TaskbarArea HolonDesktopPrivate::taskbarArea() const
{
    return d.taskbarArea;
}

int HolonDesktopPrivate::taskbarPreferedHeight() const
{
    return d.taskbarPreferedHeight;
}

int HolonDesktopPrivate::taskbarPreferedWidth() const
{
    return d.taskbarPreferedWidth;
}

QString HolonDesktopPrivate::taskbarStyleSheet() const
{
    return d.taskbarStyleSheet;
}

QList<HolonWindow *> HolonDesktopPrivate::windowList() const
{
    return d.windowList;
}

void HolonDesktopPrivate::removeDockWidget(HolonSidebarDock *dock)
{
    d.sidebars.mainWindowByDock.value(dock)->removeDockWidget(dock);
    dock->d.currentSidebar()->d_func()->setChecked(false);
}

void HolonDesktopPrivate::resizeDocks()
{
    QMapIterator it(d.sidebars.dockWidth);
    while(it.hasNext())
    {
        it.next();
        if (d.sidebars.mainWindowByDock.value(it.key()) == d.externalMainWindow &&
            it.key()->isVisible())
        {
            d.sidebars.visible.dockList.append(it.key());
            d.sidebars.visible.widthList.append(it.value());
        }
    }

    d.externalMainWindow->resizeDocks(d.sidebars.visible.dockList,
                                      d.sidebars.visible.widthList,
                                      Qt::Horizontal);

    d.sidebars.visible.dockList.clear();
    d.sidebars.visible.widthList.clear();
}

void HolonDesktopPrivate::restoreDockWidget(HolonSidebarDock *dock)
{
    d.sidebars.mainWindowByDock.value(dock)->restoreDockWidget(dock);
    dock->d.currentSidebar()->d_func()->setChecked(true);
}

void HolonDesktopPrivate::saveDockWidgetWidth(HolonSidebarDock *dock, int width)
{
    d.sidebars.dockWidth[dock] = width;

    if (!d.skipExternalMainWindowSaveState)
    {
        q_ptr->setValue("externalMainWindowState", d.externalMainWindow->saveState());
        q_ptr->setValue("internalMainWindowState", d.internalMainWindow->saveState());
    }
}

HolonSidebarDock *HolonDesktopPrivate::sidebarDock(HolonSidebar *sidebar) const
{
    if (d.sidebars.dockBySidebar.contains(sidebar))
        return d.sidebars.dockBySidebar.value(sidebar);

    return nullptr;
}
