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
#include "holonsidebarswitch.h"
#include <QLayout>
#include <QLoaderTree>
#include <QShortcut>

void HolonDesktopPrivate::addWidget(QWidget *widget, QWidget *parent)
{
    parent->layout()->addWidget(widget);
}

void HolonDesktopPrivate::setHBoxLayout(QWidget *&widget, const char *name, QWidget *parent)
{
    widget = new QWidget(parent);
    widget->setObjectName(name);
    widget->setLayout(new QHBoxLayout(widget));
    widget->layout()->setContentsMargins({});
    widget->layout()->setSpacing(0);
    parent->layout()->addWidget(widget);
}

void HolonDesktopPrivate::setMainWindow(HolonMainWindow *&widget, QWidget *parent)
{
    widget = new HolonMainWindow(q_ptr, parent);
    widget->setObjectName("ScreenCenter");
    parent->layout()->addWidget(widget);

    QObject::connect(widget, &HolonMainWindow::layoutChanged, widget, [widget, this]
    {
        if (!skipMainWindowSaveState)
            q_ptr->setValue("mainWindowState", widget->saveState());
    });
}

void HolonDesktopPrivate::setTaskbar()
{
    HolonTaskbar::Area area = taskbarArea();
    QWidget *parent{};

    switch (area) {
    case HolonTaskbar::Top: parent = top; break;
    case HolonTaskbar::Bottom: parent = bottom; break;
    case HolonTaskbar::Left: parent = left; break;
    case HolonTaskbar::Right: parent = right; break;
    }

    int preferedWidth = taskbarPreferedWidth();
    int preferedHeight = taskbarPreferedHeight();
    QString styleSheet = taskbarStyleSheet();

    taskbar = new HolonTaskbar(area,
                               preferedWidth,
                               preferedHeight,
                               styleSheet,
                               q_ptr);

    addWidget(taskbar, parent);
}

void HolonDesktopPrivate::setVBoxLayout()
{
    q_ptr->setLayout(new QVBoxLayout(q_ptr));
    q_ptr->layout()->setContentsMargins({});
    q_ptr->layout()->setSpacing(0);
}

void HolonDesktopPrivate::setVBoxLayout(QWidget *&widget, const char *name, QWidget *parent)
{
    widget = new QWidget(parent);
    widget->setObjectName(name);
    widget->setLayout(new QVBoxLayout(widget));
    widget->layout()->setContentsMargins({});
    widget->layout()->setSpacing(0);
    parent->layout()->addWidget(widget);
}

HolonTaskbar::Area HolonDesktopPrivate::taskbarArea() const
{
    QString string = q_ptr->value("taskbarArea").toString();
    if (string == "left")
        return HolonTaskbar::Left;

    if (string == "right")
        return HolonTaskbar::Right;

    if (string == "top")
        return HolonTaskbar::Top;

    return HolonTaskbar::Bottom;
}

int HolonDesktopPrivate::taskbarPreferedHeight() const
{
    return q_ptr->value("taskbarPreferedHeight", 40).toInt();
}

int HolonDesktopPrivate::taskbarPreferedWidth() const
{
    return q_ptr->value("taskbarPreferedWidth", 50).toInt();
}

QString HolonDesktopPrivate::taskbarStyleSheet() const
{
    return q_ptr->value("taskbarStyleSheet").toString();
}

HolonDesktopPrivate::HolonDesktopPrivate(HolonDesktop *q)
:   q_ptr(q),
    buttonStyleSheet(q->value("buttonStyleSheet").toString()),
    menuStyleSheet(q->value("menuStyleSheet").toString()),
    menuBorder([this]()
    {
        QRegularExpressionMatch match = borderWidth.match(menuStyleSheet);
        if (match.hasMatch())
            return match.captured("px").toInt();

        return 1;
    }()),
    menuWidth(q->value("menuWidth", 200).toInt()),
    sidebarMoveShortcut(q->value("sidebarMoveShortcut").toString()),
    titleBarHeight(q->value("titleBarHeight", 10).toInt()),
    titleBarStyleSheet(q ->value("titleBarStyleSheet").toString())
{
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
    sidebarList.append(sidebar);
    mainWindow->addSidebar(sidebar);
    taskbar->sidebarSwitch()->addSidebar(sidebar);
    mainWindow->restoreState(q_ptr->value("mainWindowState").toByteArray());
}

void HolonDesktopPrivate::setLayout()
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
