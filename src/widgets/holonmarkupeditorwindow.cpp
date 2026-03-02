// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonmarkupeditorwindow.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonid.h"
#include "holontaskstackedwindow.h"
#include "holontoolbar.h"
#include <QIcon>
#include <QLabel>
#include <QLoaderMarkupEditor>
#include <QLoaderTree>

using namespace Qt::Literals::StringLiterals;

class HolonMarkupEditorWindowPrivate
{
public:
    HolonMarkupEditorWindow *const q_ptr;
    QLoaderSettings *const settings;
    QLoaderMarkupEditor *editor{};
    HolonToolBar *toolbar{};

    HolonMarkupEditorWindowPrivate(HolonMarkupEditorWindow *q = nullptr, QLoaderSettings *s = nullptr)
    :   q_ptr(q),
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
:   HolonAbstractTaskWindow(settings, parent),
    d_ptr(this, settings)
{
    parent->addWindow(this);
}

HolonMarkupEditorWindow::HolonMarkupEditorWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractTaskWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonMarkupEditorWindow::HolonMarkupEditorWindow(QLoaderSettings *settings, HolonTaskStackedWindow *parent)
:   HolonAbstractTaskWindow(settings, parent),
    d_ptr(this, settings)
{
    parent->addWindow(this);
}

HolonMarkupEditorWindow::~HolonMarkupEditorWindow()
{ }

QWidget *HolonMarkupEditorWindow::centralWidget()
{
    return d_ptr->centralWidget();
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
    return d_ptr->toolbarWidget();
}
