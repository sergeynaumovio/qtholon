// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonparameterswindow.h"
#include "holondesktop.h"
#include "holonid.h"
#include "holonparameterswindow_p.h"
#include "holonsidebar.h"
#include <QLoaderTree>

using namespace Qt::Literals::StringLiterals;

HolonParametersWindow::HolonParametersWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonTaskAttributesWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonParametersWindow::HolonParametersWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonTaskAttributesWindow(*new HolonParametersWindowPrivate(this, parent->desktop()), settings, parent)
{
    parent->addWindow(this);
}

HolonParametersWindow::~HolonParametersWindow()
{ }

QIcon HolonParametersWindow::icon() const
{
    return {};
}

bool HolonParametersWindow::isCopyable(QStringView to) const
{
    QStringView toParent = HolonId::parentSection(to);

    if (toParent.size())
    {
        if (QObject *parent = tree()->object(toParent))
            if (qobject_cast<HolonSidebar *>(parent))
                return true;
    }

    return false;
}

QString HolonParametersWindow::title() const
{
    return u"Parameters"_s;
}

QWidget *HolonParametersWindow::toolbarWidget()
{
    Q_D(HolonParametersWindow);
    return d->toolbarWidget();
}

