// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemestyle_p.h"
#include "holonthemestyle.h"

using namespace Qt::Literals::StringLiterals;

HolonThemeStylePrivate::HolonThemeStylePrivate(HolonThemeStyle *q, HolonTheme *t)
:   q_ptr(q),
    theme(t)
{
    Q_UNUSED(q_ptr)
}
