// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonpythonbindings.h"
#include <QCoreApplication>
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

bool HolonPythonBindings::init()
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
    PyObject *module = PyImport_ImportModule("holon");
    const bool pyErrorOccurred = (PyErr_Occurred() != nullptr);

    if (module != nullptr && !pyErrorOccurred)
        Py_DECREF(module);
    else
    {
        if (pyErrorOccurred)
            PyErr_Print();

        qWarning("Failed to initialize the module.");
        return false;
    }

    return true;
}

bool HolonPythonBindings::bind(int objectIndex, const char *pythonObjectName, void *object)
{
    const char *moduleName = "__main__";
    PyTypeObject *typeObject = SbkholonTypes[objectIndex * 2];
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
