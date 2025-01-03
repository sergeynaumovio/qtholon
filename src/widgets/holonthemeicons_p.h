// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
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
    QIcon indicatorArrowDown;
    QIcon indicatorArrowUp;
    QIcon maximize;
    QIcon minimize;
    QIcon splitButtonCloseBottom;
    QIcon splitButtonCloseLeft;
    QIcon splitButtonCloseRight;
    QIcon splitButtonCloseTop;
    QIcon splitHorizontal;
    QIcon splitHorizontalMenu;
    QIcon splitVertical;
    QIcon splitVerticalMenu;
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
    QIcon indicatorArrowDownIcon();
    QIcon indicatorArrowUpIcon();
    QIcon maximizeIcon();
    QIcon minimizeIcon();
    QIcon splitButtonCloseBottomIcon();
    QIcon splitButtonCloseLeftIcon();
    QIcon splitButtonCloseRightIcon();
    QIcon splitButtonCloseTopIcon();
    QIcon splitButtonHorizontalIcon();
    QIcon splitButtonHorizontalMenuIcon();
    QIcon splitButtonVerticalIcon();
    QIcon splitButtonVerticalMenuIcon();
    QIcon taskIcon();
};

#endif // HOLONTHEMEICONS_P_H
