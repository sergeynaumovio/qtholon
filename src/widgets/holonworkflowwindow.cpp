// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowwindow.h"
#include "holonabstractwindow_p.h"
#include "holondesktop.h"
#include "holontitlebar.h"
#include "holonwindowarea.h"
#include "holonworkflow.h"
#include "holonworkflowscene.h"
#include <QBoxLayout>
#include <QComboBox>
#include <QGraphicsView>
#include <QIcon>
#include <QLabel>
#include <QLoaderTree>
#include <QScrollBar>

using namespace Qt::Literals::StringLiterals;

class HolonWorkflowWindowPrivate : public HolonAbstractWindowPrivate
{
public:
    HolonWorkflowScene *scene;
    QGraphicsView *view{};

    HolonWorkflowWindowPrivate(HolonWorkflowWindow *q, HolonDesktop *desk)
    :   HolonAbstractWindowPrivate(q, desk)
    { }

    void addComboboxItems()
    {
        QComboBox *combobox = titleBar->windowComboBox();

        if (HolonWorkflow *root = desktop->findChild<HolonWorkflow *>())
            for (HolonWorkflow *workflow : root->findChildren<HolonWorkflow *>())
                combobox->addItem(workflow->title(), QVariant::fromValue(workflow));
    }

    QWidget *centralWidget()
    {
        if (!q_ptr)
            return nullptr;

        if (view)
            return view;

        scene = new HolonWorkflowScene(q_ptr);
        view = new QGraphicsView(scene);
        view->setFrameStyle(QFrame::NoFrame);
        view->setAlignment(Qt::AlignLeft | Qt::AlignTop);

        return view;
    }
};

HolonWorkflowWindow::HolonWorkflowWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonWorkflowWindow::HolonWorkflowWindow(QLoaderSettings *settings, HolonWindowArea *parent)
:   HolonAbstractWindow(*new HolonWorkflowWindowPrivate(this, parent->desktop ()), settings, parent)
{
    parent->addWindow(this);

    Q_D(HolonWorkflowWindow);
    d->addComboboxItems();
}

HolonWorkflowWindow::~HolonWorkflowWindow()
{ }

QWidget *HolonWorkflowWindow::centralWidget() const
{
    return static_cast<HolonWorkflowWindowPrivate *>(d_ptr.get())->centralWidget();
}

QIcon HolonWorkflowWindow::icon() const
{
    return {};
}

bool HolonWorkflowWindow::isCopyable(const QStringList &to) const
{
    QStringList parentSection = to;
    if (to.size() > 1)
    {
        parentSection.removeLast();
        QObject *parent = tree()->object(parentSection);
        if (qobject_cast<HolonWindowArea *>(parent))
            return true;
    }

    return false;
}

QString HolonWorkflowWindow::title() const
{
    return u"Workflow"_s;
}

QWidget *HolonWorkflowWindow::toolbarWidget() const
{
    return {};
}
