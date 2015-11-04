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

#ifndef QDBUSMETATYPE_H
#define QDBUSMETATYPE_H

#include "QtCore/qmetatype.h"
#include <QtDBus/qdbusargument.h>

#ifndef QT_NO_DBUS

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(DBus)

class Q_DBUS_EXPORT QDBusMetaType
{
public:
    typedef void (*MarshallFunction)(QDBusArgument &, const void *);
    typedef void (*DemarshallFunction)(const QDBusArgument &, void *);

    static void registerMarshallOperators(int typeId, MarshallFunction, DemarshallFunction);
    static bool marshall(QDBusArgument &, int id, const void *data);
    static bool demarshall(const QDBusArgument &, int id, void *data);

    static int signatureToType(const char *signature);
    static const char *typeToSignature(int type);
};

template<typename T>
void qDBusMarshallHelper(QDBusArgument &arg, const T *t)
{ arg << *t; }

template<typename T>
void qDBusDemarshallHelper(const QDBusArgument &arg, T *t)
{ arg >> *t; }

template<typename T>
int qDBusRegisterMetaType(T * /* dummy */ = 0
)
{
    void (*mf)(QDBusArgument &, const T *) = qDBusMarshallHelper<T>;
    void (*df)(const QDBusArgument &, T *) = qDBusDemarshallHelper<T>;

    int id = qRegisterMetaType<T>(); // make sure it's registered
    QDBusMetaType::registerMarshallOperators(id,
        reinterpret_cast<QDBusMetaType::MarshallFunction>(mf),
        reinterpret_cast<QDBusMetaType::DemarshallFunction>(df));
    return id;
}

QT_END_NAMESPACE

QT_END_HEADER

#endif // QT_NO_DBUS
#endif
