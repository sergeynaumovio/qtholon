// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsettingswindow.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include "holonstackedwidget.h"
#include "holonworkflowmodel.h"
#include <QBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QStackedWidget>
#include <QLoaderTree>
#include <QUuid>

using namespace Qt::Literals::StringLiterals;

class HolonSettingsWindowPrivate
{
public:
    HolonSettingsWindow *const q_ptr;
    QLoaderSettings *const settings;
    HolonDesktop *const desktop;
    HolonWorkflowModel *workflowModel{};
    HolonWindowAreaStackedWidget *stackedWidget{};


    HolonSettingsWindowPrivate(HolonSettingsWindow *q = nullptr,
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

        stackedWidget = new HolonWindowAreaStackedWidget(q_ptr->group());

        return stackedWidget;
    }
};

HolonSettingsWindow::HolonSettingsWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonSettingsWindow::HolonSettingsWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonAbstractWindow(settings, parent),
    d_ptr(this, settings, parent->desktop())
{
    parent->addWindow(this);
}

HolonSettingsWindow::~HolonSettingsWindow()
{ }

Holon::WindowFlags HolonSettingsWindow::flags() const
{
    return Holon::SidebarWindow;
}

QIcon HolonSettingsWindow::icon() const
{
    return {};
}

bool HolonSettingsWindow::isCopyable(const QStringList &to) const
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

QString HolonSettingsWindow::title() const
{
    return u"Settings"_s;
}

QWidget *HolonSettingsWindow::toolbar() const
{
    return {};
}

QWidget *HolonSettingsWindow::widget(const QString &group) const
{
    if (group == QString())
        return d_ptr->widget();

    return {};
}
