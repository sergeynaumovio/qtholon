// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holoncustomtask.h"
#include "holondesktop.h"
#include "holontaskfolder.h"
#include "holonworkflow.h"
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>

using namespace Qt::Literals::StringLiterals;

class HolonCustomTaskParametersWidget : public QWidget
{
    HolonCustomTask *const q_ptr;

public:
    HolonCustomTaskParametersWidget(HolonCustomTask *q)
    :   q_ptr(q)
    {
        QVBoxLayout *layout = new QVBoxLayout(this);
        {
            auto addParameter = [=, this](const QString &parameterName, const QString &propertyName)
            {
                QHBoxLayout *hbox = new QHBoxLayout;
                {
                    QLineEdit *edit = new QLineEdit(this);
                    edit->setText(q_ptr->value(propertyName).toString());

                    connect(edit, &QLineEdit::textChanged, this, [propertyName, this](const QString &text)
                    {
                        q_ptr->setValue(propertyName, text);
                    });

                    hbox->addWidget(new QLabel(parameterName + u":", this));
                    hbox->addWidget(edit);
                    layout->addLayout(hbox);
                }
            };

            addParameter(u"Parameter A"_s, u"a"_s);
            addParameter(u"Parameter B"_s, u"b"_s);
            addParameter(u"Parameter C"_s, u"c"_s);
            layout->addStretch();
        }
    }
};

class HolonCustomTaskPrivate
{
    HolonCustomTask *const q_ptr;
    HolonCustomTaskParametersWidget *parameters{};

public:
    HolonCustomTaskPrivate(HolonCustomTask *q = nullptr)
    :   q_ptr(q)
    { }

    QWidget *parametersWidget()
    {
        if (!q_ptr)
            return nullptr;

        if (parameters)
            return parameters;

        parameters = new HolonCustomTaskParametersWidget(q_ptr);

        return parameters;
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

