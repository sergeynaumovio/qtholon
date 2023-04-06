// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTHEMEICONS_P_H
#define HOLONTHEMEICONS_P_H

#include <QIcon>

class HolonTheme;
class HolonThemeColors;
class HolonThemeIcons;
class HolonThemeStyleSheets;

class HolonThemeIconsPrivate
{
    HolonThemeIcons *const q_ptr;

    QIcon closeBackground;
    QIcon closeForeground;
    QIcon dirClosed;
    QIcon dirOpen;
    QIcon maximize;
    QIcon minimize;
    QIcon splitButtonCloseBottom;
    QIcon splitButtonCloseLeft;
    QIcon splitButtonCloseRight;
    QIcon splitButtonCloseTop;
    QIcon splitHorizontal;
    QIcon splitVertical;
    QIcon task;

public:
    HolonTheme *const theme;

    HolonThemeIconsPrivate(HolonThemeIcons *q, HolonTheme *t);

    QIcon closeBackgroundIcon();
    QIcon closeForegroundIcon();
    QIcon dirClosedIcon();
    QIcon dirOpenIcon();
    QIcon maximizeIcon();
    QIcon minimizeIcon();
    QIcon splitButtonCloseBottomIcon();
    QIcon splitButtonCloseLeftIcon();
    QIcon splitButtonCloseRightIcon();
    QIcon splitButtonCloseTopIcon();
    QIcon splitButtonHorizontalIcon();
    QIcon splitButtonVerticalIcon();
    QIcon taskIcon();
};

#endif // HOLONTHEMEICONS_P_H
