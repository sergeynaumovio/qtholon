// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsidebar.h"
#include "holondesktop.h"
#include "holonsidebar_p.h"
#include <QMainWindow>

using namespace Qt::Literals::StringLiterals;

HolonSidebar::HolonSidebar(QLoaderSettings *settings, HolonDesktop *desktop)
:   HolonWindowArea(*new HolonSidebarPrivate(this, desktop), settings)
{
    d_ptr->mainWindow->setParent(this);

    Q_D(HolonSidebar);
    d->mainWindowNestingIndex = [=, this]() -> int
    {
        QVariant v = value(u"mainWindowNestingIndex"_s);
        if (v.canConvert<uint>())
        {
            uint index = v.toUInt();
            if (index <= 127)
                return index;

            emitError(u"mainWindowNestingIndex = "_s + v.toString() + u" out of range 0..127"_s);
        }
        else
            emitError(u"mainWindowNestingIndex = "_s + v.toString() + u" not valid"_s);

        setParent(desktop);

        return -1;
    }();

    if (d->mainWindowNestingIndex < 0)
        return;

    desktop->addSidebar(this);
}

HolonSidebar::~HolonSidebar()
{ }

HolonDesktop *HolonSidebar::desktop() const
{
    return d_ptr->desktop;
}

QString HolonSidebar::group() const
{
    return value(u"group"_s).toString();
}

int HolonSidebar::mainWindowNestingIndex() const
{
    Q_D(const HolonSidebar);
    return d->mainWindowNestingIndex;
}

QByteArray HolonSidebar::mainWindowNestingState() const
{
    return value(u"mainWindowNestingState"_s).toByteArray();
}

Qt::Orientation HolonSidebar::orientation() const
{
    QString area = value(u"area"_s).toString();
    return (area == "left"_L1 || area == "right"_L1 ? Qt::Vertical : Qt::Horizontal);
}
