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

#ifndef QDESIGNER_SERVER_H
#define QDESIGNER_SERVER_H

#include <QtCore/QObject>

QT_BEGIN_NAMESPACE

class QTcpServer;
class QTcpSocket;

class QDesignerServer: public QObject
{
    Q_OBJECT
public:
    explicit QDesignerServer(QObject *parent = 0);
    virtual ~QDesignerServer();

    quint16 serverPort() const;

    static void sendOpenRequest(int port, const QStringList &files);

private slots:
    void handleNewConnection();
    void readFromClient();
    void socketClosed();

private:
    QTcpServer *m_server;
    QTcpSocket *m_socket;
};

class QDesignerClient: public QObject
{
    Q_OBJECT
public:
    explicit QDesignerClient(quint16 port, QObject *parent = 0);
    virtual ~QDesignerClient();

private slots:
    void readFromSocket();

private:
    QTcpSocket *m_socket;
};

QT_END_NAMESPACE

#endif // QDESIGNER_SERVER_H
