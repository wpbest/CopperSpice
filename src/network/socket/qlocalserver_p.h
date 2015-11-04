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

#ifndef QLOCALSERVER_P_H
#define QLOCALSERVER_P_H

#ifndef QT_NO_LOCALSERVER

#include "qlocalserver.h"
#include <qqueue.h>

#if defined(QT_LOCALSOCKET_TCP)
#   include <qtcpserver.h>

#elif defined(Q_OS_WIN)
#   include <qt_windows.h>
#   include <qwineventnotifier_p.h>

#else
#   include <qabstractsocketengine_p.h>
#   include <qsocketnotifier.h>
#endif

QT_BEGIN_NAMESPACE

class QLocalServerPrivate
{
    Q_DECLARE_PUBLIC(QLocalServer)

public:
    QLocalServerPrivate() :
#if !defined(QT_LOCALSOCKET_TCP) && !defined(Q_OS_WIN)
            listenSocket(-1), socketNotifier(0),
#endif
            maxPendingConnections(30), error(QAbstractSocket::UnknownSocketError)
    {
    }

    virtual ~QLocalServerPrivate() {}

    void init();
    bool listen(const QString &name);
    static bool removeServer(const QString &name);
    void closeServer();
    void waitForNewConnection(int msec, bool *timedOut);
    void _q_onNewConnection();

#if defined(QT_LOCALSOCKET_TCP)

    QTcpServer tcpServer;
    QMap<quintptr, QTcpSocket*> socketMap;

#elif defined(Q_OS_WIN)
    struct Listener {
        HANDLE handle;
        OVERLAPPED overlapped;
        bool connected;
    };

    void setError(const QString &function);
    bool addListener();

    QList<Listener> listeners;
    HANDLE eventHandle;
    QWinEventNotifier *connectionEventNotifier;
#else
    void setError(const QString &function);

    int listenSocket;
    QSocketNotifier *socketNotifier;
#endif

    QString serverName;
    QString fullServerName;
    int maxPendingConnections;
    QQueue<QLocalSocket*> pendingConnections;
    QString errorString;
    QAbstractSocket::SocketError error;

protected:
	 QLocalServer *q_ptr;

};

QT_END_NAMESPACE

#endif // QT_NO_LOCALSERVER

#endif // QLOCALSERVER_P_H

