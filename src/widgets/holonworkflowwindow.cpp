// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowwindow.h"
#include "holonabstracttask.h"
#include "holonabstractwindow_p.h"
#include "holondesktop.h"
#include "holonid.h"
#include "holonthemeicons.h"
#include "holontitlebar.h"
#include "holontoolbar.h"
#include "holonwindowarea.h"
#include "holonworkflow.h"
#include "holonworkflowgraphicsscene.h"
#include "holonworkflowitem.h"
#include "holonworkflowmodel.h"
#include <QComboBox>
#include <QGraphicsView>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QLoaderTree>
#include <QStackedWidget>
#include <QToolButton>
#include <QTreeView>

using namespace Qt::Literals::StringLiterals;

class HolonWorkflowWindowPrivate : public HolonAbstractWindowPrivate
{
    Q_DECLARE_PUBLIC(HolonWorkflowWindow)

public:
    QStackedWidget *stacked{};
    QHash<HolonWorkflow *, QStackedWidget *> viewByWorkflow;
    HolonToolBar *toolbar{};

    HolonWorkflowWindowPrivate(HolonWorkflowWindow *q, HolonDesktop *desk)
    :   HolonAbstractWindowPrivate(q, desk)
    { }

    void addComboboxItems()
    {
        QComboBox *combobox = titleBar->windowComboBox();
        Q_Q(HolonWorkflowWindow);
        int id = q->current();

        if (HolonWorkflow *root = desktop->findChild<HolonWorkflow *>())
            for (HolonWorkflow *workflow : root->findChildren<HolonWorkflow *>())
            {
                combobox->addItem(workflow->title(), QVariant::fromValue(workflow));

                if (workflow->objectName().toInt() == id)
                    combobox->setCurrentText(workflow->title());
            }
    }

    QWidget *centralWidget()
    {
        if (!q_ptr)
            return nullptr;

        if (stacked)
            return stacked;

        stacked = new QStackedWidget;

        if (HolonWorkflow *root = desktop->findChild<HolonWorkflow *>())
        {
            for (HolonWorkflow *workflow : root->findChildren<HolonWorkflow *>())
            {
                QStackedWidget *stackedView = new QStackedWidget;

                HolonWorkflowGraphicsScene *scene = new HolonWorkflowGraphicsScene(workflow);
                QGraphicsView *graphView = new QGraphicsView(scene);
                graphView->setFrameStyle(QFrame::NoFrame);
                graphView->setAlignment(Qt::AlignLeft | Qt::AlignTop);


                QTreeView *treeView = new QTreeView;
                treeView->setFrameStyle(QFrame::NoFrame);
                treeView->setModel(workflow->model());
                treeView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
                treeView->header()->hide();

                QObject::connect(treeView, &QTreeView::clicked, desktop, [=, this](const QModelIndex &index)
                {
                    HolonWorkflowItem *clickedItem = static_cast<HolonWorkflowItem *>(index.internalPointer());
                    QString taskId = QString::number(clickedItem->taskId());

                    if (HolonAbstractTask *clickedTask = workflow->findChild<HolonAbstractTask *>(taskId))
                        desktop->setTask(clickedTask);
                });


                stackedView->addWidget(graphView);
                stackedView->addWidget(treeView);

                Q_Q(HolonWorkflowWindow);

                if (q->isTreeView())
                    stackedView->setCurrentWidget(treeView);

                stacked->addWidget(stackedView);
                viewByWorkflow.insert(workflow, stackedView);
            }
        }

        return stacked;
    }

    QWidget *toolbarWidget()
    {
        if (!q_ptr)
            return nullptr;

        if (toolbar)
            return toolbar;

        toolbar = new HolonToolBar;

        if (stacked)
        {
            HolonThemeIcons *icons = desktop->theme()->icons();
            Q_Q(HolonWorkflowWindow);

            QToolButton *treeViewButton = toolbar->addToolButton(icons->expandIcon(), u"Tree View"_s);
            {
                treeViewButton->setCheckable(true);

                if (q->isTreeView())
                    treeViewButton->setChecked(true);

                QObject::connect(treeViewButton, &QToolButton::clicked, q, [=, this]()
                {
                    QStackedWidget *stackedView = static_cast<QStackedWidget *>(stacked->currentWidget());
                    stackedView->setCurrentIndex(treeViewButton->isChecked());
                    q->setTreeView(treeViewButton->isChecked());
                });
            }

            QComboBox *combobox = titleBar->windowComboBox();
            QObject::connect(combobox, &QComboBox::currentIndexChanged, stacked, [=, this]()
            {
                HolonWorkflow *currentWorkflow = combobox->currentData().value<HolonWorkflow *>();
                stacked->setCurrentWidget(viewByWorkflow.value(currentWorkflow));
                QStackedWidget *stackedView = static_cast<QStackedWidget *>(stacked->currentWidget());
                stackedView->setCurrentIndex(treeViewButton->isChecked());
                q->setCurrent(currentWorkflow->objectName().toInt());
            });
        }

        return toolbar;
    }
};

HolonWorkflowWindow::HolonWorkflowWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonWorkflowWindow::HolonWorkflowWindow(QLoaderSettings *settings, HolonWindowArea *parent)
:   HolonAbstractWindow(*new HolonWorkflowWindowPrivate(this, parent->desktop()), settings, parent)
{
    parent->addWindow(this);

    Q_D(HolonWorkflowWindow);
    d->addComboboxItems();
}

HolonWorkflowWindow::~HolonWorkflowWindow()
{ }

QWidget *HolonWorkflowWindow::centralWidget()
{
    Q_D(HolonWorkflowWindow);
    return d->centralWidget();
}

int HolonWorkflowWindow::current() const
{
    return value(u"current"_s).toInt();
}

QIcon HolonWorkflowWindow::icon() const
{
    return {};
}

bool HolonWorkflowWindow::isCopyable(QStringView to) const
{
    QStringView toParent = HolonId::parentSection(to);

    if (toParent.size())
    {
        if (QObject *parent = tree()->object(toParent))
            if (qobject_cast<HolonWindowArea *>(parent))
                return true;
    }

    return false;
}

bool HolonWorkflowWindow::isTreeView() const
{
    return value(u"treeView"_s).toBool();
}

void HolonWorkflowWindow::setCurrent(int id)
{
    setValue(u"current"_s, id);
}

void HolonWorkflowWindow::setTreeView(bool value)
{
    setValue(u"treeView"_s, value);
}

QString HolonWorkflowWindow::title() const
{
    return u"Workflow"_s;
}

QWidget *HolonWorkflowWindow::toolbarWidget()
{
    Q_D(HolonWorkflowWindow);
    return d->toolbarWidget();
}
