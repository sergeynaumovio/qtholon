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
#include "holonsidebardock.h"
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
    QList<HolonSidebar*> sidebarList;
    const QString sidebarMoveShortcut;
    const HolonDesktopPrivate::Area taskbarArea;
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
    HolonMainWindow *mainWindow;
    int skipMainWindowSaveState{2};
    HolonTaskbar *taskbar;

    HolonDesktopPrivateData(HolonDesktopPrivate &d, HolonDesktop *q);

    void addSidebar(HolonSidebar *sidebar);
    void addWidget(QWidget *widget, QWidget *parent);
    void resizeEvent();
    void setHBoxLayout(QWidget *&widget, const char *name, QWidget *parent);
    void setLayout();
    void setMainWindow(HolonMainWindow *&widget, QWidget *parent);
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
    taskbarArea([this]()
    {
        QString string = q_ptr->value("taskbarArea").toString();
        if (string == "left")
            return HolonDesktopPrivate::Area::Left;

        if (string == "right")
            return HolonDesktopPrivate::Area::Right;

        if (string == "top")
            return HolonDesktopPrivate::Area::Top;

        return HolonDesktopPrivate::Area::Bottom;
    }()),
    taskbarPreferedHeight(q_ptr->value("taskbarPreferedHeight", 40).toInt()),
    taskbarPreferedWidth(q_ptr->value("taskbarPreferedWidth", 50).toInt()),
    taskbarStyleSheet(q_ptr->value("taskbarStyleSheet").toString()),
    titleBarHeight(q_ptr->value("titleBarHeight", 10).toInt()),
    titleBarStyleSheet(q_ptr ->value("titleBarStyleSheet").toString())
{ }

void HolonDesktopPrivateData::addSidebar(HolonSidebar *sidebar)
{
    sidebarList.append(sidebar);
    mainWindow->addSidebar(sidebar);
    mainWindow->restoreState(q_ptr->value("mainWindowState").toByteArray());
    taskbar->sidebarSwitch()->addSidebar(sidebar);
}

void HolonDesktopPrivateData::addWidget(QWidget *widget, QWidget *parent)
{
    parent->layout()->addWidget(widget);
}

void HolonDesktopPrivateData::resizeEvent()
{
    if (skipMainWindowSaveState)
        skipMainWindowSaveState--;
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
            setMainWindow(mainWindow, middle);
            setHBoxLayout(right, "ScreenRight", middle);
        }
        setVBoxLayout(bottom, "ScreenBottom", screen);
    }
    setTaskbar();
}

void HolonDesktopPrivateData::setMainWindow(HolonMainWindow *&widget, QWidget *parent)
{
    widget = new HolonMainWindow(desktop_d, parent);
    widget->setObjectName("ScreenCenter");
    parent->layout()->addWidget(widget);
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
    if (d.skipMainWindowSaveState)
        d.skipMainWindowSaveState--;
}

void HolonDesktopPrivate::setLayout()
{
    d.setLayout();
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

HolonTaskbar *HolonDesktopPrivate::taskbar() const
{
    return d.taskbar;
}

HolonDesktopPrivate::Area HolonDesktopPrivate::taskbarArea() const
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

void HolonDesktopPrivate::saveState()
{
    if (!d.skipMainWindowSaveState)
        q_ptr->setValue("mainWindowState", d.mainWindow->saveState());
}
