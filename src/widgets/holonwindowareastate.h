// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWINDOWAREASTATE_H
#define HOLONWINDOWAREASTATE_H

class QByteArray;
class QMainWindow;

class HolonWindowAreaState
{
    QMainWindow *const &mainWindow;

public:
    HolonWindowAreaState(QMainWindow *const *mainWindow);

    QByteArray save();
};

#endif // HOLONWINDOWAREASTATE_H
