// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemeicons_p.h"
#include "holonthemecolors.h"
#include "holonthemeicons.h"
#include "holontheme.h"

using namespace Qt::Literals::StringLiterals;

HolonThemeIconsPrivate::HolonThemeIconsPrivate(HolonThemeIcons *q, HolonTheme *t)
:   q_ptr(q),
    theme(t)
{ }

QIcon HolonThemeIconsPrivate::splitButtonCloseBottomIcon()
{
    if (splitButtonCloseBottom.isNull())
        splitButtonCloseBottom = q_ptr->createIcon({{u":/holon/splitbutton_closebottom.xpm"_s,
                                                     colors->iconsBaseColor()}});

    return splitButtonCloseBottom;
}

QIcon HolonThemeIconsPrivate::splitButtonHorizontalIcon()
{
    if (splitHorizontal.isNull())
        splitHorizontal = q_ptr->createIcon({{u":/holon/splitbutton_horizontal.xpm"_s,
                                              colors->iconsBaseColor()}});

    return splitHorizontal;
}

QIcon HolonThemeIconsPrivate::splitButtonVerticalIcon()
{
    if (splitVertical.isNull())
        splitVertical = q_ptr->createIcon({{u":/holon/splitbutton_vertical.png"_s,
                                            colors->iconsBaseColor()}});

    return splitVertical;
}

