// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemestyle.h"
#include "holonthemestyle_p.h"
#include <QStyleFactory>

using namespace Qt::Literals::StringLiterals;

HolonThemeStyle::HolonThemeStyle(HolonTheme *theme) :
#ifdef Q_OS_MAC
    QProxyStyle(QStyleFactory::create(QApplication::style()->objectName())),
#else
    QProxyStyle(QStyleFactory::create(u"fusion"_s)),
#endif
    d_ptr(new HolonThemeStylePrivate(this, theme))
{ }

HolonThemeStyle::~HolonThemeStyle()
{ }
