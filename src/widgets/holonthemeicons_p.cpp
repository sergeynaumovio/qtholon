// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemeicons_p.h"
#include "holonthemecolors.h"
#include "holontheme.h"
#include <QApplication>
#include <QStyle>

using namespace Qt::Literals::StringLiterals;

QIcon HolonThemeIconsPrivate::icon(const QList<HolonMaskColor<FileName> > &masks, QIcon &value)
{
    if (value.isNull())
        value = theme->createIcon(masks);

    return value;
}

HolonThemeIconsPrivate::HolonThemeIconsPrivate(HolonThemeIcons *q, HolonTheme *t)
:   q_ptr(q),
    theme(t)
{ }

QIcon HolonThemeIconsPrivate::closeBackgroundIcon()
{
    return icon({{u":/holon/close.xpm"_s,
                  theme->colors()->panelTextColorLight()}}, closeBackground);
}

QIcon HolonThemeIconsPrivate::closeForegroundIcon()
{
    return icon({{u":/holon/close.xpm"_s,
                  theme->colors()->panelTextColorDark()}}, closeForeground);
}

QIcon HolonThemeIconsPrivate::dirClosedIcon()
{
    if (dirClosed.isNull())
    {
        int px = QApplication::style()->pixelMetric(QStyle::PM_ListViewIconSize);
        QSize iconsSize(px, px);

#ifdef Q_OS_WINDOWS
        dirClosed.addPixmap(QApplication::style()->standardIcon(QStyle::SP_DirIcon).pixmap(iconsSize));
#else
        dirClosed.addPixmap(QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon).pixmap(iconsSize));
#endif

    }

    return dirClosed;
}

QIcon HolonThemeIconsPrivate::dirOpenIcon()
{
    if (dirOpen.isNull())
    {
        int px = QApplication::style()->pixelMetric(QStyle::PM_ListViewIconSize);
        QSize iconsSize(px, px);

#ifdef Q_OS_WINDOWS
        dirOpen.addPixmap(QApplication::style()->standardIcon(QStyle::SP_DirIcon).pixmap(iconsSize));
#else
        dirOpen.addPixmap(QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon).pixmap(iconsSize));
#endif

    }

    return dirOpen;
}

QIcon HolonThemeIconsPrivate::indicatorArrowDownIcon()
{
    return icon({{u":/holon/indicator_arrowdown.xpm"_s,
                  theme->colors()->iconsBaseColor()}}, indicatorArrowDown);
}

QIcon HolonThemeIconsPrivate::indicatorArrowUpIcon()
{
    return icon({{u":/holon/indicator_arrowup.xpm"_s,
                  theme->colors()->iconsBaseColor()}}, indicatorArrowUp);
}

QIcon HolonThemeIconsPrivate::maximizeIcon()
{
    return icon({{u":/holon/maximize.xpm"_s,
                  theme->colors()->iconsBaseColor()}}, maximize);
}

QIcon HolonThemeIconsPrivate::minimizeIcon()
{
    return icon({{u":/holon/minimize.xpm"_s,
                  theme->colors()->iconsBaseColor()}}, minimize);
}

QIcon HolonThemeIconsPrivate::splitButtonCloseBottomIcon()
{
    return icon({{u":/holon/splitbutton_closebottom.xpm"_s,
                  theme->colors()->iconsBaseColor()}}, splitButtonCloseBottom);
}

QIcon HolonThemeIconsPrivate::splitButtonCloseLeftIcon()
{
    return icon({{u":/holon/splitbutton_closeleft.xpm"_s,
                  theme->colors()->iconsBaseColor()}}, splitButtonCloseLeft);
}

QIcon HolonThemeIconsPrivate::splitButtonCloseRightIcon()
{
    return icon({{u":/holon/splitbutton_closeright.xpm"_s,
                  theme->colors()->iconsBaseColor()}}, splitButtonCloseRight);
}

QIcon HolonThemeIconsPrivate::splitButtonCloseTopIcon()
{
    return icon({{u":/holon/splitbutton_closetop.xpm"_s,
                  theme->colors()->iconsBaseColor()}}, splitButtonCloseTop);
}

QIcon HolonThemeIconsPrivate::splitButtonHorizontalIcon()
{
    return icon({{u":/holon/splitbutton_horizontal.xpm"_s,
                  theme->colors()->iconsBaseColor()}}, splitHorizontal);
}

QIcon HolonThemeIconsPrivate::splitButtonVerticalIcon()
{
    return icon({{u":/holon/splitbutton_vertical.xpm"_s,
                  theme->colors()->iconsBaseColor()}}, splitVertical);
}

QIcon HolonThemeIconsPrivate::taskIcon()
{
    if (task.isNull())
    {
        int px = QApplication::style()->pixelMetric(QStyle::PM_ListViewIconSize);
        QSize iconsSize(px, px);

        task.addPixmap(QApplication::style()->standardIcon(QStyle::SP_FileIcon).pixmap(iconsSize));;
    }

    return task;
}
