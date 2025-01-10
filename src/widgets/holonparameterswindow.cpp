// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonparameterswindow.h"
#include "holondesktop.h"
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

bool HolonParametersWindow::isCopyable(const QStringList &to) const
{
    QStringList parentSection = to;
    if (to.size() > 1)
    {
        parentSection.removeLast();
        QObject *parent = tree()->object(parentSection);
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
    return {};
}

