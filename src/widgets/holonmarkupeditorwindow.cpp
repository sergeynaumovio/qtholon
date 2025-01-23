// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonmarkupeditorwindow.h"
#include "holonabstracttask.h"
#include "holonabstracttaskwindow_p.h"
#include "holondesktop.h"
#include "holonid.h"
#include "holontaskstackedwindow.h"
#include "holontoolbar.h"
#include <QIcon>
#include <QLabel>
#include <QLoaderMarkupEditor>
#include <QLoaderTree>

using namespace Qt::Literals::StringLiterals;

static HolonAbstractTask *task(HolonStackedWindow *parent)
{
    return qobject_cast<HolonAbstractTask *>(parent->parent());
}

class HolonMarkupEditorWindowPrivate : public HolonAbstractTaskWindowPrivate
{
public:
    QLoaderSettings *const settings;
    QLoaderMarkupEditor *editor{};
    HolonToolBar *toolbar{};

    HolonMarkupEditorWindowPrivate(HolonMarkupEditorWindow *q = nullptr,
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

        if (editor)
            return editor;

        editor = new QLoaderMarkupEditor(settings);
        editor->setFrameStyle(QFrame::NoFrame);

        return editor;
    }

    QWidget *toolbarWidget()
    {
        if (!q_ptr)
            return nullptr;

        if (toolbar)
            return toolbar;

        toolbar = new HolonToolBar;

        return toolbar;
    }
};

HolonMarkupEditorWindow::HolonMarkupEditorWindow(QLoaderSettings *settings, HolonAbstractTask *parent)
:   HolonAbstractTaskWindow(*new HolonMarkupEditorWindowPrivate(this, parent->desktop(), parent, settings), settings, parent)
{
    parent->addWindow(this);
}

HolonMarkupEditorWindow::HolonMarkupEditorWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractTaskWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonMarkupEditorWindow::HolonMarkupEditorWindow(QLoaderSettings *settings, HolonTaskStackedWindow *parent)
:   HolonAbstractTaskWindow(*new HolonMarkupEditorWindowPrivate(this, parent->desktop(), ::task(parent), settings), settings, parent)
{
    parent->addWindow(this);
}

HolonMarkupEditorWindow::~HolonMarkupEditorWindow()
{ }

QWidget *HolonMarkupEditorWindow::centralWidget()
{
    Q_D(HolonMarkupEditorWindow);
    return d->centralWidget();
}

QIcon HolonMarkupEditorWindow::icon() const
{
    return {};
}

bool HolonMarkupEditorWindow::isCopyable(QStringView to) const
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

QWidget *HolonMarkupEditorWindow::settingsWidget()
{
    return new QLabel(section());
}

QString HolonMarkupEditorWindow::title() const
{
    return u"Markup Editor"_s;
}

QWidget *HolonMarkupEditorWindow::toolbarWidget()
{
    Q_D(HolonMarkupEditorWindow);
    return d->toolbarWidget();
}
