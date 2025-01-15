// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKFOLDER_H
#define HOLONTASKFOLDER_H

#include "holonnamespace.h"
#include <QLoaderSettings>
#include <QObject>
#include <QScopedStorage>

class HolonDesktop;
class HolonTaskFolderPrivate;

class HolonTaskFolder : public QObject, public QLoaderSettings
{
    Q_OBJECT

    const QScopedStorage<HolonTaskFolderPrivate, 24> d_ptr;

public:
    explicit HolonTaskFolder(QLoaderSettings *settings, HolonDesktop *desktop);
    explicit HolonTaskFolder(QLoaderSettings *settings, HolonTaskFolder *taskFolder);
    ~HolonTaskFolder();

    HolonDesktop *desktop() const;
    virtual QIcon icon(Holon::TreeItem expanded) const;
    bool isExpanded() const;
    void setExpanded(bool expanded);
    virtual QString title() const;
};

#endif // HOLONTASKFOLDER_H
