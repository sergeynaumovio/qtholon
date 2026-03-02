// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONMARKUPEDITORWINDOW_H
#define HOLONMARKUPEDITORWINDOW_H

#include "holonabstracttaskwindow.h"
#include <QScopedStorage>

class HolonDesktop;
class HolonMarkupEditorWindowPrivate;
class HolonTaskStackedWindow;

class HolonMarkupEditorWindow : public HolonAbstractTaskWindow
{
    Q_OBJECT

    const QScopedStorage<HolonMarkupEditorWindowPrivate, 32> d_ptr;

public:
    HolonMarkupEditorWindow(QLoaderSettings *settings, HolonAbstractTask *parent);
    HolonMarkupEditorWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonMarkupEditorWindow(QLoaderSettings *settings, HolonTaskStackedWindow *parent);
    ~HolonMarkupEditorWindow();

    QWidget *centralWidget() override;
    QIcon icon() const override;
    bool isCopyable(QStringView to) const override;
    QWidget *settingsWidget() override;
    QString title() const override;
    QWidget *toolbarWidget() override;
};

#endif // HOLONMARKUPEDITORWINDOW_H
