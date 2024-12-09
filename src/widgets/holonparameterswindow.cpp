// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonparameterswindow.h"
#include "holondesktop.h"
#include "holonparameterswindow_p.h"
#include "holonsidebar.h"
#include "holonstackedwidget.h"
#include <QBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QStackedWidget>
#include <QLoaderTree>
#include <QUuid>

using namespace Qt::Literals::StringLiterals;


HolonParametersWindow::HolonParametersWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonParametersWindow::HolonParametersWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonAbstractWindow(*new HolonParametersWindowPrivate(this, parent->desktop()), settings, parent)
{
    parent->addWindow(this);
}

HolonParametersWindow::~HolonParametersWindow()
{ }

Holon::WindowFlags HolonParametersWindow::flags() const
{
    return Holon::SidebarWindow;
}

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

int HolonParametersWindow::role() const
{
    return Holon::ParametersRole;
}

QString HolonParametersWindow::title() const
{
    return u"Parameters"_s;
}

QWidget *HolonParametersWindow::toolbar() const
{
    return {};
}

QWidget *HolonParametersWindow::widget(int widgetRole) const
{
    if (widgetRole == Holon::NoRole)
        return static_cast<HolonParametersWindowPrivate *>(d_ptr.get())->widget();

    return {};
}
