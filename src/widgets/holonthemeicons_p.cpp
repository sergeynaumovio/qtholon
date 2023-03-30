// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemeicons_p.h"
#include "holonthemecolors.h"
#include "holonthemeicons.h"
#include "holontheme.h"
#include <QApplication>
#include <QStyle>

using namespace Qt::Literals::StringLiterals;

HolonThemeIconsPrivate::HolonThemeIconsPrivate(HolonThemeIcons *q, HolonTheme *t)
:   q_ptr(q),
    theme(t)
{ }

QIcon HolonThemeIconsPrivate::closeBackgroundIcon()
{
    if (closeBackground.isNull())
        closeBackground = theme->createIcon({{u":/holon/close.xpm"_s,
                                              colors->panelTextColorLight()}});

    return closeBackground;
}

QIcon HolonThemeIconsPrivate::closeForegroundIcon()
{
    if (closeForeground.isNull())
        closeForeground = theme->createIcon({{u":/holon/close.xpm"_s,
                                              colors->panelTextColorDark()}});

    return closeForeground;
}

QIcon HolonThemeIconsPrivate::dirClosedIcon()
{
    if (dir.isNull())
        dir.addPixmap(QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon).pixmap(QSize(16, 16)));

    return dir;
}

QIcon HolonThemeIconsPrivate::dirOpenIcon()
{
    if (dirOpen.isNull())
        dirOpen.addPixmap(QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon).pixmap(QSize(16, 16)));

    return dirOpen;
}

QIcon HolonThemeIconsPrivate::maximizeIcon()
{
    if (maximize.isNull())
        maximize = theme->createIcon({{u":/holon/maximize.xpm"_s,
                                       colors->iconsBaseColor()}});

    return maximize;
}

QIcon HolonThemeIconsPrivate::minimizeIcon()
{
    if (minimize.isNull())
        minimize = theme->createIcon({{u":/holon/minimize.xpm"_s,
                                       colors->iconsBaseColor()}});

    return minimize;
}

QIcon HolonThemeIconsPrivate::splitButtonCloseBottomIcon()
{
    if (splitButtonCloseBottom.isNull())
        splitButtonCloseBottom = theme->createIcon({{u":/holon/splitbutton_closebottom.xpm"_s,
                                                     colors->iconsBaseColor()}});

    return splitButtonCloseBottom;
}

QIcon HolonThemeIconsPrivate::splitButtonCloseLeftIcon()
{
    if (splitButtonCloseLeft.isNull())
        splitButtonCloseLeft = theme->createIcon({{u":/holon/splitbutton_closeleft.xpm"_s,
                                                   colors->iconsBaseColor()}});

    return splitButtonCloseLeft;
}

QIcon HolonThemeIconsPrivate::splitButtonCloseRightIcon()
{
    if (splitButtonCloseRight.isNull())
        splitButtonCloseRight = theme->createIcon({{u":/holon/splitbutton_closeright.xpm"_s,
                                                    colors->iconsBaseColor()}});

    return splitButtonCloseRight;
}

QIcon HolonThemeIconsPrivate::splitButtonCloseTopIcon()
{
    if (splitButtonCloseTop.isNull())
        splitButtonCloseTop = theme->createIcon({{u":/holon/splitbutton_closetop.xpm"_s,
                                                  colors->iconsBaseColor()}});

    return splitButtonCloseTop;
}

QIcon HolonThemeIconsPrivate::splitButtonHorizontalIcon()
{
    if (splitHorizontal.isNull())
        splitHorizontal = theme->createIcon({{u":/holon/splitbutton_horizontal.xpm"_s,
                                              colors->iconsBaseColor()}});

    return splitHorizontal;
}

QIcon HolonThemeIconsPrivate::splitButtonVerticalIcon()
{
    if (splitVertical.isNull())
        splitVertical = theme->createIcon({{u":/holon/splitbutton_vertical.xpm"_s,
                                            colors->iconsBaseColor()}});

    return splitVertical;
}

