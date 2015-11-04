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

#ifndef QDBUSTREENODE_H
#define QDBUSTREENODE_H

#include <QtDBus/qdbusmacros.h>
#include <QtCore/qstring.h>
#include <QtCore/qobject.h>

#ifndef QT_NO_DBUS

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(DBus)

class QDBusMessage;
class QDBusConnection;

class QDBusVirtualObjectPrivate;
class Q_DBUS_EXPORT QDBusVirtualObject : public QObject
{
    Q_OBJECT
public:
    explicit QDBusVirtualObject(QObject *parent = 0);
    virtual ~QDBusVirtualObject();

    virtual QString introspect(const QString &path) const = 0;
    virtual bool handleMessage(const QDBusMessage &message, const QDBusConnection &connection) = 0;

private:
    Q_DECLARE_PRIVATE(QDBusVirtualObject)
    Q_DISABLE_COPY(QDBusVirtualObject)
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QT_NO_DBUS
#endif
