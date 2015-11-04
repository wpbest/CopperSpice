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

#ifndef QUNIXSOCKETSERVER_P_H
#define QUNIXSOCKETSERVER_P_H

#include <QtCore/qobject.h>

QT_BEGIN_NAMESPACE

class QUnixSocketServerPrivate;

class Q_GUI_EXPORT QUnixSocketServer : public QObject
{
    CS_OBJECT(QUnixSocketServer)

public:
    enum ServerError { NoError, InvalidPath, ResourceError, BindError,
                       ListenError };

    QUnixSocketServer(QObject *parent=0);
    virtual ~QUnixSocketServer();

    void close();

    ServerError serverError() const;

    bool isListening() const;
    bool listen(const QByteArray & path);

    int socketDescriptor() const;
    QByteArray serverAddress() const;
   
    int maxPendingConnections() const; 
    void setMaxPendingConnections(int numConnections);

protected:
    virtual void incomingConnection(int socketDescriptor) = 0;

private:
    QUnixSocketServer(const QUnixSocketServer &);
    QUnixSocketServer & operator=(const QUnixSocketServer &);

    friend class QUnixSocketServerPrivate;
    QUnixSocketServerPrivate * d;
};


QT_END_NAMESPACE
#endif // QUNIXSOCKETSERVER_P_H

