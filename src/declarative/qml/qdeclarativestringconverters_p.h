/***********************************************************************
*
* Copyright (c) 2012-2014 Barbara Geller
* Copyright (c) 2012-2014 Ansel Sermersheim
* Copyright (c) 2012-2014 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef QDECLARATIVESTRINGCONVERTERS_P_H
#define QDECLARATIVESTRINGCONVERTERS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qglobal.h>
#include <QtCore/qvariant.h>

#include <private/qdeclarativeglobal_p.h>

QT_BEGIN_NAMESPACE

class QColor;
class QPointF;
class QSizeF;
class QRectF;
class QString;
class QByteArray;
class QVector3D;

// XXX - Bauhaus currently uses these methods which is why they're exported
namespace QDeclarativeStringConverters
{
    QVariant Q_DECLARATIVE_PRIVATE_EXPORT variantFromString(const QString &);
    QVariant Q_DECLARATIVE_PRIVATE_EXPORT variantFromString(const QString &, int preferredType, bool *ok = 0);

    QColor Q_DECLARATIVE_PRIVATE_EXPORT colorFromString(const QString &, bool *ok = 0);
#ifndef QT_NO_DATESTRING
    QDate Q_DECLARATIVE_PRIVATE_EXPORT dateFromString(const QString &, bool *ok = 0); 
    QTime Q_DECLARATIVE_PRIVATE_EXPORT timeFromString(const QString &, bool *ok = 0);
    QDateTime Q_DECLARATIVE_PRIVATE_EXPORT dateTimeFromString(const QString &, bool *ok = 0);
#endif
    QPointF Q_DECLARATIVE_PRIVATE_EXPORT pointFFromString(const QString &, bool *ok = 0);
    QSizeF Q_DECLARATIVE_PRIVATE_EXPORT sizeFFromString(const QString &, bool *ok = 0);
    QRectF Q_DECLARATIVE_PRIVATE_EXPORT rectFFromString(const QString &, bool *ok = 0);
    QVector3D Q_DECLARATIVE_PRIVATE_EXPORT vector3DFromString(const QString &, bool *ok = 0);
}

QT_END_NAMESPACE

#endif // QDECLARATIVESTRINGCONVERTERS_P_H
