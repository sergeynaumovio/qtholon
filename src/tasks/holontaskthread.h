// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKTHREAD_H
#define HOLONTASKTHREAD_H

#include "qtholonglobal.h"
#include <QThread>

class HolonDesktop;
class HolonTaskThreadPrivate;

class Q_HOLON_EXPORT HolonTaskThread : public QThread
{
    Q_OBJECT

    const QScopedPointer<HolonTaskThreadPrivate> d_ptr;

protected:
    void run() override;

public:
    HolonTaskThread(HolonDesktop *desktop);
    ~HolonTaskThread();
};

#endif // HOLONTASKTHREAD_H
