// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemestylesheets_p.h"
#include "holonthemestylesheets.h"

using namespace Qt::Literals::StringLiterals;

HolonThemeStyleSheetsPrivate::HolonThemeStyleSheetsPrivate(HolonThemeStyleSheets *q, HolonTheme *t)
:   q_ptr(q),
    theme(t)
{ }

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
