// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTHEMEICONS_P_H
#define HOLONTHEMEICONS_P_H

#include "holontheme.h"
#include <QIcon>

class HolonThemeColors;
class HolonThemeIcons;
class HolonThemeStyleSheets;

class HolonThemeIconsPrivate
{
    QIcon clear;
    QIcon closeBackground;
    QIcon closeForeground;
    QIcon dirClosed;
    QIcon dirOpen;
    QIcon expand;
    QIcon indicatorArrowDown;
    QIcon indicatorArrowUp;
    QIcon maximize;
    QIcon minimize;
    QIcon run;
    QIcon runTask;
    QIcon splitButtonCloseBottom;
    QIcon splitButtonCloseLeft;
    QIcon splitButtonCloseRight;
    QIcon splitButtonCloseTop;
    QIcon splitHorizontal;
    QIcon splitHorizontalMenu;
    QIcon splitVertical;
    QIcon splitVerticalMenu;
    QIcon stop;
    QIcon task;

    QIcon icon(const QList<HolonMaskColor<FileName>> &masks, QIcon &value);

public:
    HolonTheme *const theme;

    HolonThemeIconsPrivate(HolonTheme *t);

    QIcon clearIcon();
    QIcon closeBackgroundIcon();
    QIcon closeForegroundIcon();
    QIcon dirClosedIcon();
    QIcon dirOpenIcon();
    QIcon expandIcon();
    QIcon indicatorArrowDownIcon();
    QIcon indicatorArrowUpIcon();
    QIcon maximizeIcon();
    QIcon minimizeIcon();
    QIcon runIcon();
    QIcon runTaskIcon();
    QIcon splitButtonCloseBottomIcon();
    QIcon splitButtonCloseLeftIcon();
    QIcon splitButtonCloseRightIcon();
    QIcon splitButtonCloseTopIcon();
    QIcon splitButtonHorizontalIcon();
    QIcon splitButtonHorizontalMenuIcon();
    QIcon splitButtonVerticalIcon();
    QIcon splitButtonVerticalMenuIcon();
    QIcon stopIcon();
    QIcon taskIcon();
};

#endif // HOLONTHEMEICONS_P_H
