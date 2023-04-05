// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemestylesheets_p.h"
#include "holonthemestylesheets.h"

using namespace Qt::Literals::StringLiterals;

HolonThemeStyleSheetsPrivate::HolonThemeStyleSheetsPrivate(HolonThemeStyleSheets *q, HolonTheme *t)
:   q_ptr(q),
    theme(t)
{ }

QString HolonThemeStyleSheetsPrivate::buttonStyleSheet()
{
    if (!button.isNull())
        return button;

    if (!(button = q_ptr->value(u"buttonStyleSheet"_s).toString()).isNull())
        return button;

    button = u"QPushButton { border: 0px; }"
             u"QPushButton:hover { background-color: rgb(74, 76, 78); color: white; }"
             u"QPushButton::menu-indicator { width: 0px; }"_s;

    return button;
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
