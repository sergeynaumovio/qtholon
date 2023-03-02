// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonterminalwindow.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include <QBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLoaderTerminal>
#include <QLoaderTree>

class HolonTerminalWindowPrivate
{
public:
    HolonTerminalWindow *const q_ptr;
    QLoaderSettings *const settings;
    bool close{true};
    QWidget *terminal{};

    HolonTerminalWindowPrivate(HolonTerminalWindow *q, QLoaderSettings *s)
    :   q_ptr(q),
        settings(s)
    { }

    QWidget *widget()
    {
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
:   HolonAbstractWindow(settings, parent),
    d_ptr(new HolonTerminalWindowPrivate(this, settings))
{
    parent->addWindow(this);
}

HolonTerminalWindow::HolonTerminalWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent),
    d_ptr(new HolonTerminalWindowPrivate(this, settings))
{
    parent->addWindow(this);
}

HolonTerminalWindow::~HolonTerminalWindow()
{ }

HolonAbstractWindow::Area HolonTerminalWindow::area() const
{
    return HolonAbstractWindow::Central;
}

HolonAbstractWindow::Attributes HolonTerminalWindow::attributes() const
{
    return HolonAbstractWindow::WindowCloseButtonHint;
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
        if (qobject_cast<HolonAbstractTask *>(parent))
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

QWidget *HolonTerminalWindow::widget(const QString &group) const
{
    if (group == QString())
        return d_ptr->widget();

    if (group == HolonAbstractWindow::group())
        return new QLabel(section().constLast());

    return {};
}
