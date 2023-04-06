// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemesizehints_p.h"
#include "holonthemesizehints.h"

using namespace Qt::Literals::StringLiterals;

HolonThemeSizeHintsPrivate::HolonThemeSizeHintsPrivate(HolonThemeSizeHints *q, HolonTheme *t)
:   q_ptr(q),
    theme(t)
{ }

QSize HolonThemeSizeHintsPrivate::titleBarSizeHint()
{
    if (titleBar.isValid())
        return titleBar;

    if ((titleBar = q_ptr->value(u"titleBarSizeHint"_s).toSize()).isValid())
        return titleBar;

    titleBar = QSize(24, 24);

    return titleBar;
}
