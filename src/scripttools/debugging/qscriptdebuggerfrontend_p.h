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

#ifndef QSCRIPTDEBUGGERFRONTEND_P_H
#define QSCRIPTDEBUGGERFRONTEND_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qobjectdefs.h>
#include <QtCore/qscopedpointer.h>
#include <QtCore/qstring.h>

#include "qscriptdebuggercommandschedulerinterface_p.h"

QT_BEGIN_NAMESPACE

class QScriptDebuggerEventHandlerInterface;
class QScriptDebuggerCommand;
class QScriptDebuggerEvent;
class QScriptDebuggerResponse;

class QScriptDebuggerFrontendPrivate;
class QScriptDebuggerFrontend
    : public QScriptDebuggerCommandSchedulerInterface
{
public:
    QScriptDebuggerFrontend();
    virtual ~QScriptDebuggerFrontend();

    QScriptDebuggerEventHandlerInterface *eventHandler() const;
    void setEventHandler(QScriptDebuggerEventHandlerInterface *eventHandler);

    int scheduleCommand(const QScriptDebuggerCommand &command,
                        QScriptDebuggerResponseHandlerInterface *responseHandler);

    int scheduledCommandCount() const;

protected:
    void notifyCommandFinished(int id, const QScriptDebuggerResponse &response);
    bool notifyEvent(const QScriptDebuggerEvent &event);

    virtual void processCommand(int id, const QScriptDebuggerCommand &command) = 0;

protected:
    QScriptDebuggerFrontend(QScriptDebuggerFrontendPrivate &dd);
    QScopedPointer<QScriptDebuggerFrontendPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(QScriptDebuggerFrontend)
    Q_DISABLE_COPY(QScriptDebuggerFrontend)
};

QT_END_NAMESPACE

#endif
