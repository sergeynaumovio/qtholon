// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonparameterswindow.h"
#include "holondesktop.h"
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

class HolonParametersWindowPrivate
{
public:
    HolonParametersWindow *const q_ptr;
    QLoaderSettings *const settings;
    HolonDesktop *const desktop;
    HolonTaskStackedWidget *stackedWidget{};


    HolonParametersWindowPrivate(HolonParametersWindow *q = nullptr,
                                 QLoaderSettings *s = nullptr,
                                 HolonDesktop *desk = nullptr)
    :   q_ptr(q),
        settings(s),
        desktop(desk)
    { }

    QWidget *widget()
    {
        if (!q_ptr)
            return nullptr;

        if (stackedWidget)
            return stackedWidget;

        stackedWidget = new HolonTaskStackedWidget(q_ptr->role());

        return stackedWidget;
    }
};

HolonParametersWindow::HolonParametersWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonParametersWindow::HolonParametersWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonAbstractWindow(settings, parent),
    d_ptr(this, settings, parent->desktop())
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
        return d_ptr->widget();

    return {};
}
