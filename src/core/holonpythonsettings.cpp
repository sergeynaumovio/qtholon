// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonpythonsettings.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include <QCoreApplication>
#include <QLoaderTree>
#include <QOperatingSystemVersion>
#include <sbkconverter.h>
#include <sbkmodule.h>

using namespace Qt::Literals::StringLiterals;

extern "C" PyObject *PyInit_holon();
extern PyTypeObject **SbkholonTypes;
extern SbkConverter **SbkholonTypeConverters;

static const char builtinModuleName[] = "holon";
static const char virtualEnvVar[] = "VIRTUAL_ENV";

static void pyFinalize()
{
    Py_Finalize();
}

static void initVirtualEnvironment()
{
    if (QOperatingSystemVersion::currentType() == QOperatingSystemVersion::Windows &&
        (PY_MAJOR_VERSION > 3 || (PY_MAJOR_VERSION == 3 && PY_MINOR_VERSION >= 8)))
    {
        const QByteArray virtualEnvPath = qgetenv(virtualEnvVar);
        qputenv("PYTHONPATH", virtualEnvPath + "\\Lib\\site-packages");
    }
}

static bool init()
{
    if (qEnvironmentVariableIsSet(virtualEnvVar))
        initVirtualEnvironment();

    if (PyImport_AppendInittab(builtinModuleName, PyInit_holon) == -1)
    {
        qWarning("Failed to add the module '%s' to the table of built-in modules.", builtinModuleName);
        return false;
    }

    Py_Initialize();
    qAddPostRoutine(pyFinalize);

    if (!PyInit_holon() || PyErr_Occurred())
    {
        qWarning("Failed to initialize the module.");
        return false;
    }

    return true;
}

static bool bind(HolonPythonSettings *object)
{
    const char *moduleName = "__main__";
    const char *pythonObjectName = "settings";

    PyTypeObject *typeObject = SbkholonTypes[0];

    PyObject *pyObject = Shiboken::Conversions::pointerToPython(typeObject, object);
    if (!pyObject)
    {
        qWarning() << __FUNCTION__ << "Failed to create wrapper for" << object;

        return false;
    }
    Py_INCREF(pyObject);

    PyObject *module = PyImport_AddModule(moduleName);
    if (!module)
    {
        Py_DECREF(pyObject);

        if (PyErr_Occurred())
            PyErr_Print();

        qWarning() << __FUNCTION__ << "Failed to locate module" << moduleName;

        return false;
    }

    if (PyModule_Add(module, pythonObjectName, pyObject) < 0)
    {
        if (PyErr_Occurred())
            PyErr_Print();

        qWarning() << __FUNCTION__ << "Failed add object" << pythonObjectName << "to" << moduleName;

        return false;
    }

    return true;
}

bool HolonPythonSettings::init(HolonDesktop *desk)
{
    desktop = desk;

    if (!::init() || !bind(this))
        return false;

    return true;
}

bool HolonPythonSettings::setValue(const QString &key, const QVariant &value)
{
    return desktop->task()->setValue(key, value);
}

QVariant HolonPythonSettings::value(const QString &key, const QVariant &defaultValue) const
{
    return desktop->task()->value(key, defaultValue);
}
