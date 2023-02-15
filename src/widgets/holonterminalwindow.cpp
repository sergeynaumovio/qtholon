// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonterminalwindow.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include <QBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLoaderTerminal>
#include <QLoaderTree>

class HolonTerminalWindowPrivate
{
public:
    HolonTerminalWindow *const q_ptr;
    QLoaderSettings *const settings{};
    HolonSidebar *const sidebar{};
    bool close{true};
    QWidget *w{};

    HolonTerminalWindowPrivate(HolonTerminalWindow *q, QLoaderSettings *s, HolonSidebar *sb = nullptr)
    :   q_ptr(q),
        settings(s),
        sidebar(sb)
    { }

    QWidget *widget()
    {
        if (!w)
        {
            w = new QLoaderTerminal(settings);
            QObject::connect(sidebar, &QObject::destroyed, q_ptr, [this] { close = false; });
            QObject::connect(w, &QObject::destroyed, q_ptr, [this] { if (close) q_ptr->close(); });
        }

        return w;
    }
};

HolonTerminalWindow::HolonTerminalWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonWindow(settings, parent),
    d_ptr(new HolonTerminalWindowPrivate(this, settings))
{ }

HolonTerminalWindow::HolonTerminalWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonWindow(settings, parent),
    d_ptr(new HolonTerminalWindowPrivate(this, settings, parent))
{
    parent->addWindow(this);
}

HolonTerminalWindow::~HolonTerminalWindow()
{ }

HolonWindow::Areas HolonTerminalWindow::areas() const
{
    return HolonWindow::Sidebar;
}

HolonWindow::Attributes HolonTerminalWindow::attributes() const
{
    return HolonWindow::WindowCloseButtonHint;
}

QIcon HolonTerminalWindow::icon() const
{
    return {};
}

bool HolonTerminalWindow::isCopyable(const QStringList &to) const
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

QString HolonTerminalWindow::title() const
{
    return "Terminal";
}

QWidget *HolonTerminalWindow::toolbar() const
{
    return {};
}

QWidget *HolonTerminalWindow::widget() const
{
    return d_ptr->widget();
}
