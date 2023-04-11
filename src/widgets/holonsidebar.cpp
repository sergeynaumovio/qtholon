// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsidebar.h"
#include "holondesktop.h"
#include "holonsidebar_p.h"
#include <QMainWindow>

using namespace Qt::Literals::StringLiterals;

HolonSidebar::HolonSidebar(QLoaderSettings *settings, HolonDesktop *desktop)
:   HolonWindowArea(*new HolonSidebarPrivate(desktop, this), settings)
{
    d_ptr->mainWindow->setParent(this);

    Q_D(HolonSidebar);
    d->mainWindowNestingIndex = [=, this]()
    {
        QVariant v = value(u"mainWindowNestingIndex"_s);

        bool ok;
        int index = v.toInt(&ok);

        if (ok)
        {
            if (index >= 0 && index <= 127)
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

QByteArray HolonSidebar::mainWindowState() const
{
    return value(u"mainWindowState"_s).toByteArray();
}
