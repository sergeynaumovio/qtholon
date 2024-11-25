// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holoncore.h"
#include "holoncore_p.h"

HolonCore::HolonCore(QLoaderSettings *settings, QObject *parent)
:   QObject(parent),
    QLoaderSettings(this, settings),
    d_ptr(new HolonCorePrivate)
{ }

HolonCore::~HolonCore()
{ }
