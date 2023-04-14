// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemesizehints_p.h"
#include "holonthemesizehints.h"

using namespace Qt::Literals::StringLiterals;

HolonThemeSizeHintsPrivate::HolonThemeSizeHintsPrivate(HolonThemeSizeHints *q, HolonTheme *t)
:   q_ptr(q),
    theme(t)
{ }

QSize HolonThemeSizeHintsPrivate::sidebarSwitchButtonSizeHint()
{
    if (sidebarSwitchButton.isValid())
        return sidebarSwitchButton;

    if ((sidebarSwitchButton = q_ptr->value(u"sidebarSwitchButtonSizeHint"_s).toSize()).isValid())
        return sidebarSwitchButton;

    sidebarSwitchButton = QSize(120, 26);

    return sidebarSwitchButton;
}

QSize HolonThemeSizeHintsPrivate::taskbarSizeHint()
{
    if (taskbar.isValid())
        return taskbar;

    if ((taskbar = q_ptr->value(u"taskbarSizeHint"_s).toSize()).isValid())
        return taskbar;

    taskbar = QSize(50, 26);

    return titleBar;
}

QSize HolonThemeSizeHintsPrivate::titleBarSizeHint()
{
    if (titleBar.isValid())
        return titleBar;

    if ((titleBar = q_ptr->value(u"titleBarSizeHint"_s).toSize()).isValid())
        return titleBar;

    titleBar = QSize(24, 24);

    return titleBar;
}
