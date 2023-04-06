// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemestylesheets_p.h"
#include "holonthemestylesheets.h"

using namespace Qt::Literals::StringLiterals;

HolonThemeStyleSheetsPrivate::HolonThemeStyleSheetsPrivate(HolonThemeStyleSheets *q, HolonTheme *t)
:   q_ptr(q),
    theme(t)
{ }

QString HolonThemeStyleSheetsPrivate::mainWindowStyleSheet()
{
    if (!mainWindow.isNull())
        return mainWindow;

    if (!(mainWindow = q_ptr->value(u"taskbarStyleSheet"_s).toString()).isNull())
        return mainWindow;

    mainWindow = u"QMainWindow::separator { width: 1px; height: 1px; background-color: rgb(34, 36, 38); }"_s;

    return mainWindow;
}

QString HolonThemeStyleSheetsPrivate::pushButtonStyleSheet()
{
    if (!pushButton.isNull())
        return pushButton;

    if (!(pushButton = q_ptr->value(u"pushButtonStyleSheet"_s).toString()).isNull())
        return pushButton;

    pushButton = u"QPushButton { border: 0px; }"
                 u"QPushButton:hover { background-color: rgb(74, 76, 78); color: white; }"
                 u"QPushButton::menu-indicator { width: 0px; }"_s;

    return pushButton;
}

QString HolonThemeStyleSheetsPrivate::taskbarStyleSheet()
{
    if (!taskbar.isNull())
        return taskbar;

    if (!(taskbar = q_ptr->value(u"taskbarStyleSheet"_s).toString()).isNull())
        return taskbar;

    taskbar = u"QWidget { color: white; background-color: rgb(54, 56, 58); }"_s;

    return taskbar;
}

QString HolonThemeStyleSheetsPrivate::titleBarStyleSheet()
{
    if (!titleBar.isNull())
        return titleBar;

    if (!(titleBar = q_ptr->value(u"titleBarStyleSheet"_s).toString()).isNull())
        return titleBar;

    titleBar = u"QWidget { color: white; background-color: rgb(54, 56, 58); }"_s;

    return titleBar;
}
