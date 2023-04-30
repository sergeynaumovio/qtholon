// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontheme.h"
#include "holondesktop.h"
#include "holondesktop_p.h"
#include "holontheme_p.h"
#include "holonthemecolors.h"
#include "holonthemeicons.h"
#include "holonthemestyle.h"
#include <QApplication>
#include <QFileInfo>
#include <QLoaderSettings>
#include <QPainter>
#include <QRegularExpression>

using namespace Qt::Literals::StringLiterals;

HolonTheme::HolonTheme(QLoaderSettings *settings,
                       HolonDesktop *desktop,
                       HolonThemeColors *colors,
                       HolonThemeIcons *icons,
                       HolonThemeStyle *style)
:   QObject(desktop),
    QLoaderSettings(this, settings),
    d_ptr(new HolonThemePrivate(this, colors, icons, style, desktop))
{
    desktop->addTheme(this);
}

HolonTheme::HolonTheme(QLoaderSettings *settings, HolonDesktop *desktop)
:   QObject(desktop),
    QLoaderSettings(this, settings),
    d_ptr(new HolonThemePrivate(this, settings, desktop))
{
    desktop->addTheme(this);
}

HolonTheme::~HolonTheme()
{ }

HolonThemeColors *HolonTheme::colors() const
{
    return d_ptr->colors;
}

QIcon HolonTheme::createIcon(const QList<HolonMaskColor<FileName>> &masks) const
{
    if (masks.isEmpty())
        return {};

    auto maskFileName = [](const QString &fileName, int devicePixelRatio)
    {
        if (devicePixelRatio == 1)
            return fileName;

        QString name(fileName);
        QString suffix(QFileInfo(fileName).completeSuffix());
        QString scale(u"@"_s + QString::number(devicePixelRatio) + u"x"_s);

        if (suffix.isEmpty())
            name.append(scale);
        else
            name.replace(QRegularExpression(u"."_s + suffix + u"$"_s),
                         scale + u"."_s + suffix);

        return name;
    };

    int dpr = qRound(qApp->devicePixelRatio());

    QList<HolonMaskColor<QPixmap>> pixmaps;
    {
        for (const auto &[mask, color] : std::as_const(masks))
        {
            QString fileName = maskFileName(mask, dpr);
            QPixmap pixmap(fileName);
            if (pixmap.isNull())
            {
                emitWarning(u"image file not valid "_s + fileName);
                continue;
            }

            pixmaps.append({pixmap, color});
        }

        if (pixmaps.isEmpty())
            return {};
    }

    auto resultMask = [&pixmaps]()
    {
        QPixmap pixmap(pixmaps.first().mask);
        QPainter p(&pixmap);
        p.setCompositionMode(QPainter::CompositionMode_Darken);

        for (int i = 1; i < pixmaps.size(); ++i)
            p.drawPixmap(0, 0, pixmaps.at(i).mask);

        return pixmap;
    };

    auto coloringOne = [](const QPixmap &pixmap, const QColor &color)
    {
        QImage image(pixmap.toImage().convertToFormat(QImage::Format_ARGB32));
        image.setDevicePixelRatio(pixmap.devicePixelRatio());

        QRgb *data = reinterpret_cast<QRgb *>(image.bits());
        QRgb tint(color.rgb() & 0x00ffffff);

        for (int i = 0, n = image.width() * image.height(); i < n; ++i)
        {
            QRgb alpha = (((~data[i]) & 0xff) * color.alpha()) >> 8;
            data[i] = (alpha << 24) | tint;
        }

        return QPixmap::fromImage(image);
    };

    auto coloringAll = [&pixmaps, &coloringOne](const QPixmap &iconMask)
    {
        QPixmap pixmap(iconMask.size());
        pixmap.setDevicePixelRatio(iconMask.devicePixelRatio());
        pixmap.fill(Qt::transparent);

        QPainter p(&pixmap);

        for (const auto &[mask, color] : std::as_const(pixmaps))
            p.drawPixmap(0, 0, coloringOne(mask, color));

        return pixmap;
    };

    QIcon icon;
    QColor disabledStateColor = colors()->iconsDisabledColor();
    QPixmap iconMask = (masks.size() == 1 ? maskFileName(masks.first().mask, dpr) : resultMask());
    icon.addPixmap(coloringOne(iconMask, disabledStateColor), QIcon::Disabled);
    icon.addPixmap(coloringAll(iconMask), QIcon::Normal);

    return icon;
}

HolonDesktop *HolonTheme::desktop() const
{
    return d_ptr->desktop;
}

HolonThemeIcons *HolonTheme::icons() const
{
    return d_ptr->icons;
}

bool HolonTheme::isCurrent() const
{
    return value(u"current"_s).toBool();
}
