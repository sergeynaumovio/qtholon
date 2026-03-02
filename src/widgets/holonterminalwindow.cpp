// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonterminalwindow.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonid.h"
#include "holontaskstackedwindow.h"
#include "holonthemeicons.h"
#include "holontoolbar.h"
#include <QIcon>
#include <QLabel>
#include <QLoaderShell>
#include <QLoaderTerminal>
#include <QLoaderTree>
#include <QToolButton>

using namespace Qt::Literals::StringLiterals;

class HolonTerminalWindowPrivate
{
public:
    HolonTerminalWindow *const q_ptr;
    QLoaderSettings *const settings;
    bool close{true};
    QLoaderTerminal *terminal{};
    QLoaderShell *shell{};
    HolonToolBar *toolbar{};

    HolonTerminalWindowPrivate(HolonTerminalWindow *q = nullptr, QLoaderSettings *s = nullptr)
    :   q_ptr(q),
        settings(s)
    { }

    QWidget *centralWidget()
    {
        if (!q_ptr)
            return nullptr;

        if (terminal)
            return terminal;

        terminal = new QLoaderTerminal(settings);
        terminal->setFrameStyle(QFrame::NoFrame);
        shell = terminal->shell();

        QObject::connect(q_ptr->desktop(), &QObject::destroyed, q_ptr, [this] { close = false; });

        QObject::connect(terminal, &QWidget::destroyed, q_ptr, [this]
        {
            if (close)
            {
                HolonAbstractWindow *stacked = qobject_cast<HolonTaskStackedWindow *>(q_ptr->parent());
                q_ptr->desktop()->closeWindow(stacked ? stacked : q_ptr);
            }
        });

        return terminal;
    }

    QWidget *toolbarWidget()
    {
        if (!q_ptr)
            return nullptr;

        if (toolbar)
            return toolbar;

        toolbar = new HolonToolBar;

        if (terminal)
        {
            HolonThemeIcons *icons = q_ptr->desktop()->theme()->icons();

            QToolButton *clearButton = toolbar->addToolButton(icons->clearIcon(), u"Clear"_s);
            QObject::connect(clearButton, &QToolButton::clicked, terminal, [this](){ terminal->clear(); });
        }

        return toolbar;
    }
};

HolonTerminalWindow::HolonTerminalWindow(QLoaderSettings *settings, HolonAbstractTask *parent)
:   HolonAbstractTaskWindow(settings, parent),
    d_ptr(this, settings)
{
    parent->addWindow(this);
}

HolonTerminalWindow::HolonTerminalWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractTaskWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonTerminalWindow::HolonTerminalWindow(QLoaderSettings *settings, HolonTaskStackedWindow *parent)
:   HolonAbstractTaskWindow(settings, parent),
    d_ptr(this, settings)
{
    parent->addWindow(this);
}

HolonTerminalWindow::~HolonTerminalWindow()
{ }

QWidget *HolonTerminalWindow::centralWidget()
{
    return d_ptr->centralWidget();
}

QIcon HolonTerminalWindow::icon() const
{
    return {};
}

bool HolonTerminalWindow::isCopyable(QStringView to) const
{
    QStringView toParent = HolonId::parentSection(to);

    if (toParent.size())
    {
        if (QObject *parent = tree()->object(toParent))
            if (qobject_cast<HolonAbstractTask *>(parent) || qobject_cast<HolonStackedWindow *>(parent))
                return true;
    }

    return false;
}

QWidget *HolonTerminalWindow::settingsWidget()
{
    return new QLabel(section());
}

QString HolonTerminalWindow::title() const
{
    return u"Terminal"_s;
}

QWidget *HolonTerminalWindow::toolbarWidget()
{
    return d_ptr->toolbarWidget();
}
