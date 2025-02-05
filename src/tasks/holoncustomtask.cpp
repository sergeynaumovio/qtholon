// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holoncustomtask.h"
#include "holondesktop.h"
#include "holontaskfolder.h"
#include "holonworkflow.h"
#include <QBoxLayout>
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickWidget>
#include <QThread>
#include <sbkpython.h>

using namespace Qt::Literals::StringLiterals;

class HolonCustomTaskPrivate
{
    HolonCustomTask *const q_ptr;
    QWidget *widget{};

public:
    HolonCustomTaskPrivate(HolonCustomTask *q = nullptr)
    :   q_ptr(q)
    { }

    QWidget *parametersWidget()
    {
        if (!q_ptr)
            return nullptr;

        if (widget)
            return widget;

        QString property(u"parametersWidget"_s);
        QString fileName(q_ptr->value(property).toString());
        QFileInfo fileInfo(fileName);

        if (fileInfo.exists())
        {
            if (fileInfo.suffix() == "py"_L1)
            {
                QFile file(fileName);

                if (file.open(QFile::ReadOnly | QFile::Text))
                {
                    widget = new QWidget;

                    QByteArray script = file.readAll();
                    PyRun_SimpleString(script);
                }
            }
            else if (fileInfo.suffix() == "qml"_L1)
            {
                QQuickWidget *view = new QQuickWidget;

                QObject::connect(view, &QQuickWidget::sceneGraphError, q_ptr, [](QQuickWindow::SceneGraphError error, const QString &message)
                {
                    qDebug() << error << message;
                });

                QObject::connect(view, &QQuickWidget::statusChanged, q_ptr, [=](QQuickWidget::Status status)
                {
                    if (status == QQuickWidget::Error)
                        qDebug() << view->errors();
                });

                QObject::connect(view->engine(), &QQmlEngine::warnings, q_ptr, [](const QList<QQmlError> &warnings)
                {
                    qDebug() << warnings;
                });

                view->rootContext()->setContextProperty(u"settings"_s, q_ptr);
                view->setSource(fileName);

                widget = view;
            }
            else
                q_ptr->emitWarning(u"file format not valid: "_s + fileName);
        }
        else
            q_ptr->emitWarning(u"file not found: "_s + fileName);

        if (!widget)
            widget = new QWidget;

        return widget;
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
    QString fileName(value(u"exec"_s).toString());
    QFileInfo fileInfo(fileName);

    if (fileInfo.exists())
    {
        if (fileInfo.suffix() == "py"_L1)
        {
            QFile file(fileName);

            if (file.open(QFile::ReadOnly | QFile::Text))
            {
                QByteArray script = file.readAll();

                PyGILState_STATE state = PyGILState_Ensure();
                PyRun_SimpleString(script);
                PyGILState_Release(state);
            }
        }
    }

    if (QThread::currentThread()->isInterruptionRequested())
        return false;

    return true;
}

QWidget *HolonCustomTask::parametersWidget()
{
    return d_ptr->parametersWidget();
}

