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

#ifndef QDECLARATIVEOBSERVERSERVICE_H
#define QDECLARATIVEOBSERVERSERVICE_H

#include "private/qdeclarativedebugservice_p.h"
#include <private/qdeclarativeglobal_p.h>

#include <QtCore/QList>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QDeclarativeView;
class QDeclarativeInspectorInterface;

class Q_DECLARATIVE_EXPORT QDeclarativeInspectorService : public QDeclarativeDebugService
{
    CS_OBJECT(QDeclarativeInspectorService)

public:
    QDeclarativeInspectorService();
    static QDeclarativeInspectorService *instance();

    void addView(QDeclarativeView *);
    void removeView(QDeclarativeView *);
    QList<QDeclarativeView*> views() const { return m_views; }

    void sendMessage(const QByteArray &message);

public:
    CS_SIGNAL_1(Public, void gotMessage(const QByteArray & message))
    CS_SIGNAL_2(gotMessage,message) 

protected:
    virtual void statusChanged(Status status);
    virtual void messageReceived(const QByteArray &);

private:
    void updateStatus();

    static QDeclarativeInspectorInterface *loadInspectorPlugin();

    QList<QDeclarativeView*> m_views;
    QDeclarativeInspectorInterface *m_inspectorPlugin;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QDECLARATIVEOBSERVERSERVICE_H
