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

#ifndef QTCPSERVERCONNECTION_H
#define QTCPSERVERCONNECTION_H

#include <QtDeclarative/private/qdeclarativedebugserverconnection_p.h>

QT_BEGIN_NAMESPACE

class QDeclarativeDebugServer;
class QTcpServerConnectionPrivate;
class QTcpServerConnection : public QObject, public QDeclarativeDebugServerConnection
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QTcpServerConnection)
    Q_DISABLE_COPY(QTcpServerConnection)
    Q_INTERFACES(QDeclarativeDebugServerConnection)


public:
    QTcpServerConnection();
    ~QTcpServerConnection();

    void setServer(QDeclarativeDebugServer *server);
    void setPort(int port, bool bock);

    bool isConnected() const;
    void send(const QByteArray &message);
    void disconnect();
    bool waitForMessage();

    void listen();
    void waitForConnection();

private Q_SLOTS:
    void readyRead();
    void newConnection();

private:
    QTcpServerConnectionPrivate *d_ptr;
};

QT_END_NAMESPACE

#endif // QTCPSERVERCONNECTION_H
