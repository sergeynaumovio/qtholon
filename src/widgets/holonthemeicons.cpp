// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemeicons.h"
#include "holontheme.h"
#include "holonthemecolors.h"
#include "holonthemeicons_p.h"
#include <QApplication>
#include <QFileInfo>
#include <QLoaderSettings>
#include <QPainter>
#include <QRegularExpression>

using namespace Qt::Literals::StringLiterals;

HolonThemeIcons::HolonThemeIcons(QLoaderSettings *settings, HolonTheme *theme)
:   QObject(theme),
    QLoaderSettings(settings),
    d_ptr(new HolonThemeIconsPrivate(this, theme))
{ }

HolonThemeIcons::~HolonThemeIcons()
{ }

HolonThemeColors *HolonThemeIcons::colors() const
{
    return d_ptr->colors;
}

QIcon HolonThemeIcons::createIcon(const QList<HolonMaskColor<FileName>> &masks) const
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

    QList<HolonMaskColor<QPixmap>> pixmaps;
    {
        int dpr = qRound(qApp->devicePixelRatio());
        for (const auto &[mask, color] : masks)
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
        QPixmap pixmap;
        QPainter p(&pixmap);
        p.setCompositionMode(QPainter::CompositionMode_Darken);

        for (const auto &[mask, color] : pixmaps)
           p.drawPixmap(0, 0, mask);

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

        for (const auto &[mask, color] : pixmaps)
            p.drawPixmap(0, 0, coloringOne(mask, color));

        return pixmap;
    };

    QIcon icon;
    QColor disabledStateColor = colors()->iconsDisabledColor();
    QPixmap iconMask = (masks.size() == 1 ? masks.first().mask : resultMask());
    icon.addPixmap(coloringOne(iconMask, disabledStateColor), QIcon::Disabled);
    icon.addPixmap(coloringAll(iconMask), QIcon::Normal);

    return icon;
}

QIcon HolonThemeIcons::splitButtonCloseBottomIcon() const
{
    return d_ptr->splitButtonCloseBottomIcon();
}

QIcon HolonThemeIcons::splitButtonHorizontalIcon() const
{
    return d_ptr->splitButtonHorizontalIcon();
}

QIcon HolonThemeIcons::splitButtonVerticalIcon() const
{
    return d_ptr->splitButtonVerticalIcon();
}

HolonTheme *HolonThemeIcons::theme() const
{
    return d_ptr->theme;
}
