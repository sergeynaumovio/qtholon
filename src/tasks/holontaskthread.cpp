// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontaskthread.h"
#include "holonabstracttask.h"
#include "holondesktop.h"

class HolonTaskThreadPrivate
{
public:
    HolonDesktop *const desktop;

    HolonTaskThreadPrivate(HolonDesktop *desk)
    :   desktop(desk)
    { }
};

void HolonTaskThread::run()
{
    d_ptr->desktop->currentTask()->exec();
}

HolonTaskThread::HolonTaskThread(HolonDesktop *desktop)
:   QThread(desktop),
    d_ptr(new HolonTaskThreadPrivate(desktop))
{ }

HolonTaskThread::~HolonTaskThread()
{ }
