/***********************************************************************
*
* Copyright (c) 2012-2013 Barbara Geller
* Copyright (c) 2012-2013 Ansel Sermersheim
* Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This file is part of CopperSpice.
*
* CopperSpice is free software: you can redistribute it and/or 
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with CopperSpice.  If not, see 
* <http://www.gnu.org/licenses/>.
*
***********************************************************************/

#include "iconloader_p.h"

#include <QtCore/QFile>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal {

QDESIGNER_SHARED_EXPORT QIcon createIconSet(const QString &name)
{
    QStringList candidates = QStringList()
        << (QString::fromUtf8(":/trolltech/formeditor/images/") + name)
#ifdef Q_WS_MAC
        << (QString::fromUtf8(":/trolltech/formeditor/images/mac/") + name)
#else
        << (QString::fromUtf8(":/trolltech/formeditor/images/win/") + name)
#endif
        << (QString::fromUtf8(":/trolltech/formeditor/images/designer_") + name);

    foreach (const QString &f, candidates) {
        if (QFile::exists(f))
            return QIcon(f);
    }

    return QIcon();
}

QDESIGNER_SHARED_EXPORT QIcon emptyIcon()
{
    return QIcon(QLatin1String(":/trolltech/formeditor/images/emptyicon.png"));
}

} // namespace qdesigner_internal

QT_END_NAMESPACE

