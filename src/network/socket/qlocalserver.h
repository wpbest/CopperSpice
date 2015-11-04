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

#ifndef QLOCALSERVER_H
#define QLOCALSERVER_H

#include <QtNetwork/qabstractsocket.h>
#include <QScopedPointer>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_LOCALSERVER

class QLocalSocket;
class QLocalServerPrivate;

class Q_NETWORK_EXPORT QLocalServer : public QObject
{
    CS_OBJECT(QLocalServer)
    Q_DECLARE_PRIVATE(QLocalServer)

public:
    NET_CS_SIGNAL_1(Public, void newConnection())
    NET_CS_SIGNAL_2(newConnection) 

    QLocalServer(QObject *parent = 0);
    ~QLocalServer();

    void close();
    QString errorString() const;
    virtual bool hasPendingConnections() const;
    bool isListening() const;
    bool listen(const QString &name);
    int maxPendingConnections() const;
    virtual QLocalSocket *nextPendingConnection();
    QString serverName() const;
    QString fullServerName() const;
    static bool removeServer(const QString &name);
    QAbstractSocket::SocketError serverError() const;
    void setMaxPendingConnections(int numConnections);
    bool waitForNewConnection(int msec = 0, bool *timedOut = 0);

protected:
    virtual void incomingConnection(quintptr socketDescriptor);
    QScopedPointer<QLocalServerPrivate> d_ptr;

private:
    Q_DISABLE_COPY(QLocalServer)

    NET_CS_SLOT_1(Private, void _q_onNewConnection())
    NET_CS_SLOT_2(_q_onNewConnection)

};

#endif // QT_NO_LOCALSERVER

QT_END_NAMESPACE

#endif // QLOCALSERVER_H

