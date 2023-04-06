// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTHEMESIZEHINTS_P_H
#define HOLONTHEMESIZEHINTS_P_H

#include <QSize>

class HolonTheme;
class HolonThemeSizeHints;

class HolonThemeSizeHintsPrivate
{
    HolonThemeSizeHints *const q_ptr;

    QSize icons;
    QSize titleBar;

public:
    HolonTheme *const theme;

    HolonThemeSizeHintsPrivate(HolonThemeSizeHints *q, HolonTheme *t);

    QSize iconsSizeHint();
    QSize titleBarSizeHint();
};

#endif // HOLONTHEMESIZEHINTS_P_H