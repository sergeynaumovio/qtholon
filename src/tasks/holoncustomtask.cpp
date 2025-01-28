// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holoncustomtask.h"
#include "holondesktop.h"
#include "holontaskfolder.h"
#include "holonworkflow.h"
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickWidget>

using namespace Qt::Literals::StringLiterals;

class HolonCustomTaskPrivate
{
    HolonCustomTask *const q_ptr;
    QQuickWidget *view{};

public:
    HolonCustomTaskPrivate(HolonCustomTask *q = nullptr)
    :   q_ptr(q)
    { }

    QWidget *parametersWidget()
    {
        if (!q_ptr)
            return nullptr;

        if (view)
            return view;

        view = new QQuickWidget;

        QObject::connect(view, &QQuickWidget::sceneGraphError, q_ptr, [](QQuickWindow::SceneGraphError error, const QString &message)
        {
            qDebug() << error << message;
        });

        QObject::connect(view, &QQuickWidget::statusChanged, q_ptr, [this](QQuickWidget::Status status)
        {
            if (status == QQuickWidget::Error)
                qDebug() << view->errors();
        });

        QObject::connect(view->engine(), &QQmlEngine::warnings, q_ptr, [](const QList<QQmlError> &warnings)
        {
            qDebug() << warnings;
        });

        view->rootContext()->setContextProperty(u"settings"_s, q_ptr);
        view->setSource(q_ptr->value(u"qml"_s).toString());

        return view;
    }
};

HolonCustomTask::HolonCustomTask(QLoaderSettings *settings, HolonTaskFolder *folder)
:   HolonAbstractTask(settings, folder)
{ }

HolonCustomTask::HolonCustomTask(QLoaderSettings *settings, HolonWorkflow *workflow)
:   HolonAbstractTask(settings, workflow),
    d_ptr(this)
{
    workflow->desktop()->addTask(this);
}

HolonCustomTask::~HolonCustomTask()
{ }

bool HolonCustomTask::exec()
{
    return true;
}

QWidget *HolonCustomTask::parametersWidget()
{
    return d_ptr->parametersWidget();
}

