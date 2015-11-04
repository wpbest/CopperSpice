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

#ifndef QSCRIPTDEBUGGERVALUE_P_H
#define QSCRIPTDEBUGGERVALUE_P_H

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

#include <QtCore/qobjectdefs.h>
#include <private/qscopedpointer_p.h>
#include <QtCore/qlist.h>

QT_BEGIN_NAMESPACE

class QScriptValue;
class QScriptEngine;
class QDataStream;

class QScriptDebuggerValuePrivate;
class QScriptDebuggerValue
{
public:
    enum ValueType {
        NoValue,
        UndefinedValue,
        NullValue,
        BooleanValue,
        StringValue,
        NumberValue,
        ObjectValue
    };

    QScriptDebuggerValue();
    QScriptDebuggerValue(const QScriptValue &value);
    QScriptDebuggerValue(double value);
    QScriptDebuggerValue(bool value);
    QScriptDebuggerValue(const QString &value);
    QScriptDebuggerValue(qint64 objectId);
    QScriptDebuggerValue(ValueType type);
    QScriptDebuggerValue(const QScriptDebuggerValue &other);
    ~QScriptDebuggerValue();

    QScriptDebuggerValue &operator=(const QScriptDebuggerValue &other);

    ValueType type() const;

    double numberValue() const;
    bool booleanValue() const;
    QString stringValue() const;
    qint64 objectId() const;

    QScriptValue toScriptValue(QScriptEngine *engine) const;
    QString toString() const;

    bool operator==(const QScriptDebuggerValue &other) const;
    bool operator!=(const QScriptDebuggerValue &other) const;

private:
    QScopedSharedPointer<QScriptDebuggerValuePrivate> d_ptr;

    Q_DECLARE_PRIVATE(QScriptDebuggerValue)
};

typedef QList<QScriptDebuggerValue> QScriptDebuggerValueList;

QDataStream &operator<<(QDataStream &, const QScriptDebuggerValue &);
QDataStream &operator>>(QDataStream &, QScriptDebuggerValue &);

QT_END_NAMESPACE

#endif
