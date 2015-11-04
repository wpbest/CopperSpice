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

#ifndef QABSTRACTMESSAGEHANDLER_H
#define QABSTRACTMESSAGEHANDLER_H

#include <QtXmlPatterns/QSourceLocation>
#include <QtCore/QObject>
#include <QScopedPointer>

QT_BEGIN_NAMESPACE

class QAbstractMessageHandlerPrivate;

class Q_XMLPATTERNS_EXPORT QAbstractMessageHandler : public QObject
{
    CS_OBJECT(QAbstractMessageHandler)

public:
    QAbstractMessageHandler(QObject *parent = 0);
    virtual ~QAbstractMessageHandler();

    void message(QtMsgType type,
                 const QString &description,
                 const QUrl &identifier = QUrl(),
                 const QSourceLocation &sourceLocation = QSourceLocation());

protected:
    virtual void handleMessage(QtMsgType type,
                               const QString &description,
                               const QUrl &identifier,
                               const QSourceLocation &sourceLocation) = 0;
    QScopedPointer<QAbstractMessageHandlerPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(QAbstractMessageHandler)
    Q_DISABLE_COPY(QAbstractMessageHandler)
	

};

QT_END_NAMESPACE

#endif
