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

#ifndef QDECLARATIVEDEBUGSERVER_H
#define QDECLARATIVEDEBUGSERVER_H

#include <private/qdeclarativeglobal_p.h>
#include <private/qdeclarativedebugserverconnection_p.h>
#include <QScopedPointer>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QDeclarativeDebugService;
class QDeclarativeDebugServerPrivate;

class Q_DECLARATIVE_EXPORT QDeclarativeDebugServer : public QObject
{
    CS_OBJECT(QDeclarativeDebugServer)

    Q_DECLARE_PRIVATE(QDeclarativeDebugServer)
    Q_DISABLE_COPY(QDeclarativeDebugServer)

public:
    virtual ~QDeclarativeDebugServer();
  
    static QDeclarativeDebugServer *instance();

    void setConnection(QDeclarativeDebugServerConnection *connection);

    bool hasDebuggingClient() const;

    QList<QDeclarativeDebugService*> services() const;
    QStringList serviceNames() const;

    bool addService(QDeclarativeDebugService *service);
    bool removeService(QDeclarativeDebugService *service);

    void sendMessage(QDeclarativeDebugService *service, const QByteArray &message);
    void receiveMessage(const QByteArray &message);

    bool waitForMessage(QDeclarativeDebugService *service);

private:
    friend class QDeclarativeDebugService;
    friend class QDeclarativeDebugServicePrivate;

    QDeclarativeDebugServer();    

    CS_SLOT_1(Private, void _q_deliverMessage(QString un_named_arg1,QByteArray un_named_arg2))
    CS_SLOT_2(_q_deliverMessage)

protected:
	 QScopedPointer<QDeclarativeDebugServerPrivate> d_ptr;

};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QDECLARATIVEDEBUGSERVICE_H
