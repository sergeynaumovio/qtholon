// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTHEMEICONS_P_H
#define HOLONTHEMEICONS_P_H

#include <QIcon>

class HolonTheme;
class HolonThemeColors;
class HolonThemeIcons;

class HolonThemeIconsPrivate
{
    QIcon splitButtonCloseBottom;
    QIcon splitHorizontal;
    QIcon splitVertical;

public:
    HolonThemeIcons *const q_ptr;
    HolonTheme *const theme;
    HolonThemeColors *colors;

    HolonThemeIconsPrivate(HolonThemeIcons *q, HolonTheme *t);

    QIcon splitButtonCloseBottomIcon();
    QIcon splitButtonHorizontalIcon();
    QIcon splitButtonVerticalIcon();
};

#endif // HOLONTHEMEICONS_P_H
