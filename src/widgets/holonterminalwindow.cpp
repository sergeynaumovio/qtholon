// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonterminalwindow.h"
#include "holonabstracttask.h"
#include "holonabstracttaskwindow_p.h"
#include "holondesktop.h"
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

static HolonAbstractTask *task(HolonStackedWindow *parent)
{
    return qobject_cast<HolonAbstractTask *>(parent->parent());
}

class HolonTerminalWindowPrivate : public HolonAbstractTaskWindowPrivate
{
public:
    QLoaderSettings *const settings;
    bool close{true};
    QLoaderTerminal *terminal{};
    QLoaderShell *shell{};
    HolonToolBar *toolbar{};

    HolonTerminalWindowPrivate(HolonTerminalWindow *q = nullptr,
                               HolonDesktop *desk = nullptr,
                               HolonAbstractTask *t = nullptr,
                               QLoaderSettings *s = nullptr)
    :   HolonAbstractTaskWindowPrivate(q, desk, t),
        settings(s)
    { }

    QWidget *centralWidget()
    {
        if (!q_ptr)
            return nullptr;

        if (terminal)
            return terminal;

        terminal = new QLoaderTerminal(settings);
        shell = terminal->shell ();

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
            HolonThemeIcons *icons = desktop->theme()->icons();

            QToolButton *clearButton = toolbar->toolButton(icons->clearIcon(), u"Clear"_s);
            {
                toolbar->addWidget(clearButton);

                QObject::connect(clearButton, &QToolButton::clicked, terminal, [this]()
                {
                    terminal->clear();
                });
            }
        }

        return toolbar;
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

HolonTerminalWindow::HolonTerminalWindow(QLoaderSettings *settings, HolonTaskStackedWindow *parent)
:   HolonAbstractTaskWindow(*new HolonTerminalWindowPrivate(this, parent->desktop(), ::task(parent), settings), settings, parent)
{
    parent->addWindow(this);
}

HolonTerminalWindow::~HolonTerminalWindow()
{ }

QWidget *HolonTerminalWindow::centralWidget() const
{
    return static_cast<HolonTerminalWindowPrivate *>(d_ptr.get())->centralWidget();
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
    return static_cast<HolonTerminalWindowPrivate *>(d_ptr.get())->toolbarWidget();
}
