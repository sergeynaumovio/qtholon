// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonterminalwindow.h"
#include "holonabstracttask.h"
#include "holonabstracttaskwindow_p.h"
#include "holonstackedwindow.h"
#include "holondesktop.h"
#include <QBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLoaderTerminal>
#include <QLoaderTree>

using namespace Qt::Literals::StringLiterals;

static HolonAbstractTask *task(HolonStackedWindow *parent)
{
    return qobject_cast<HolonAbstractTask *>(parent->parent());
}

class HolonTerminalWindowPrivate : public HolonAbstractTaskWindowPrivate
{
public:
    QLoaderSettings *const settings;
    bool close{true};
    QWidget *terminal{};

    HolonTerminalWindowPrivate(HolonTerminalWindow *q = nullptr,
                               HolonDesktop *desk = nullptr,
                               HolonAbstractTask *t = nullptr,
                               QLoaderSettings *s = nullptr)
    :   HolonAbstractTaskWindowPrivate(q, desk, t),
        settings(s)
    { }

    QWidget *widget()
    {
        if (!q_ptr)
            return nullptr;

        if (terminal)
            return terminal;

        terminal = new QLoaderTerminal(settings);

        QObject::connect(q_ptr->desktop(), &QObject::destroyed, q_ptr, [this] { close = false; });
        QObject::connect(terminal, &QWidget::destroyed, q_ptr, [this]
        {
            if (close)
                q_ptr->desktop()->closeWindow(q_ptr);
        });

        return terminal;
    }
};

HolonTerminalWindow::HolonTerminalWindow(QLoaderSettings *settings, HolonAbstractTask *parent)
:   HolonAbstractTaskWindow(*new HolonTerminalWindowPrivate(this, parent->desktop(), parent, settings), settings, parent)
{
    parent->addWindow(this);
}

HolonTerminalWindow::HolonTerminalWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractTaskWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonTerminalWindow::HolonTerminalWindow(QLoaderSettings *settings, HolonStackedWindow *parent)
    :   HolonAbstractTaskWindow(*new HolonTerminalWindowPrivate(this, parent->desktop(), ::task(parent), settings), settings, parent)
{
    parent->addWindow(this);
}

HolonTerminalWindow::~HolonTerminalWindow()
{ }

QWidget *HolonTerminalWindow::centralWidget() const
{
    return static_cast<HolonTerminalWindowPrivate *>(d_ptr.get())->widget();
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
        if (qobject_cast<HolonAbstractTask *>(parent) || qobject_cast<HolonStackedWindow *>(parent))
            return true;
    }

    return false;
}

QWidget *HolonTerminalWindow::settingsWidget() const
{
    return new QLabel(section().constLast());
}

QString HolonTerminalWindow::title() const
{
    return u"Terminal"_s;
}

QWidget *HolonTerminalWindow::toolbarWidget() const
{
    return {};
}
