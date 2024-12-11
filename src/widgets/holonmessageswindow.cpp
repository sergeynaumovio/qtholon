// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonmessageswindow.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include "holonstackedwidget.h"
#include <QIcon>
#include <QLoaderTree>

using namespace Qt::Literals::StringLiterals;

class HolonMessagesWindowPrivate
{
public:
    HolonMessagesWindow *const q_ptr;
    QLoaderSettings *const settings;
    HolonDesktop *const desktop;
    HolonTaskStackedWidget *stackedWidget{};

    HolonMessagesWindowPrivate(HolonMessagesWindow *q = nullptr,
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

HolonMessagesWindow::HolonMessagesWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonMessagesWindow::HolonMessagesWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonAbstractWindow(settings, parent),
    d_ptr(this, settings, parent->desktop())
{
    parent->addWindow(this);
}

HolonMessagesWindow::~HolonMessagesWindow()
{ }

QWidget *HolonMessagesWindow::centralWidget() const
{
    return d_ptr->widget();
}

QIcon HolonMessagesWindow::icon() const
{
    return {};
}

bool HolonMessagesWindow::isCopyable(const QStringList &to) const
{
    QStringList parentSection = to;
    if (to.size() > 1)
    {
        parentSection.removeLast();
        QObject *parent = tree()->object(parentSection);
        if (qobject_cast<HolonSidebar*>(parent))
            return true;
    }

    return false;
}

int HolonMessagesWindow::role() const
{
    return Holon::MessagesRole;
}

QString HolonMessagesWindow::title() const
{
    return u"Messages"_s;
}

QWidget *HolonMessagesWindow::toolbarWidget() const
{
    return {};
}

QWidget *HolonMessagesWindow::widget(int widgetRole) const
{
    if (widgetRole == Holon::NoRole)
        return d_ptr->widget();

    return {};
}
