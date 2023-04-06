// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemeicons_p.h"
#include "holonthemecolors.h"
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
                                              theme->colors()->panelTextColorLight()}});

    return closeBackground;
}

QIcon HolonThemeIconsPrivate::closeForegroundIcon()
{
    if (closeForeground.isNull())
        closeForeground = theme->createIcon({{u":/holon/close.xpm"_s,
                                              theme->colors()->panelTextColorDark()}});

    return closeForeground;
}

QIcon HolonThemeIconsPrivate::dirClosedIcon()
{
    if (dirClosed.isNull())
#ifdef Q_OS_WINDOWS
        dirClosed.addPixmap(QApplication::style()->standardIcon(QStyle::SP_DirIcon).pixmap(QSize(16, 16)));
#else
        dirClosed.addPixmap(QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon).pixmap(QSize(16, 16)));
#endif

    return dirClosed;
}

QIcon HolonThemeIconsPrivate::dirOpenIcon()
{
    if (dirOpen.isNull())
#ifdef Q_OS_WINDOWS
        dirOpen.addPixmap(QApplication::style()->standardIcon(QStyle::SP_DirIcon).pixmap(QSize(16, 16)));
#else
        dirOpen.addPixmap(QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon).pixmap(QSize(16, 16)));
#endif

    return dirOpen;
}

QIcon HolonThemeIconsPrivate::maximizeIcon()
{
    if (maximize.isNull())
        maximize = theme->createIcon({{u":/holon/maximize.xpm"_s,
                                       theme->colors()->iconsBaseColor()}});

    return maximize;
}

QIcon HolonThemeIconsPrivate::minimizeIcon()
{
    if (minimize.isNull())
        minimize = theme->createIcon({{u":/holon/minimize.xpm"_s,
                                       theme->colors()->iconsBaseColor()}});

    return minimize;
}

QIcon HolonThemeIconsPrivate::splitButtonCloseBottomIcon()
{
    if (splitButtonCloseBottom.isNull())
        splitButtonCloseBottom = theme->createIcon({{u":/holon/splitbutton_closebottom.xpm"_s,
                                                     theme->colors()->iconsBaseColor()}});

    return splitButtonCloseBottom;
}

QIcon HolonThemeIconsPrivate::splitButtonCloseLeftIcon()
{
    if (splitButtonCloseLeft.isNull())
        splitButtonCloseLeft = theme->createIcon({{u":/holon/splitbutton_closeleft.xpm"_s,
                                                   theme->colors()->iconsBaseColor()}});

    return splitButtonCloseLeft;
}

QIcon HolonThemeIconsPrivate::splitButtonCloseRightIcon()
{
    if (splitButtonCloseRight.isNull())
        splitButtonCloseRight = theme->createIcon({{u":/holon/splitbutton_closeright.xpm"_s,
                                                    theme->colors()->iconsBaseColor()}});

    return splitButtonCloseRight;
}

QIcon HolonThemeIconsPrivate::splitButtonCloseTopIcon()
{
    if (splitButtonCloseTop.isNull())
        splitButtonCloseTop = theme->createIcon({{u":/holon/splitbutton_closetop.xpm"_s,
                                                  theme->colors()->iconsBaseColor()}});

    return splitButtonCloseTop;
}

QIcon HolonThemeIconsPrivate::splitButtonHorizontalIcon()
{
    if (splitHorizontal.isNull())
        splitHorizontal = theme->createIcon({{u":/holon/splitbutton_horizontal.xpm"_s,
                                              theme->colors()->iconsBaseColor()}});

    return splitHorizontal;
}

QIcon HolonThemeIconsPrivate::splitButtonVerticalIcon()
{
    if (splitVertical.isNull())
        splitVertical = theme->createIcon({{u":/holon/splitbutton_vertical.xpm"_s,
                                            theme->colors()->iconsBaseColor()}});

    return splitVertical;
}

