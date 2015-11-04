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

#ifndef QSCRIPTSTDMESSAGEHANDLER_P_H
#define QSCRIPTSTDMESSAGEHANDLER_P_H


#include <QtCore/qscopedpointer.h>
#include "qscriptmessagehandlerinterface_p.h"

QT_BEGIN_NAMESPACE

class QScriptStdMessageHandlerPrivate;
class QScriptStdMessageHandler
    : public QScriptMessageHandlerInterface
{
public:
    QScriptStdMessageHandler();
    ~QScriptStdMessageHandler();

    void message(QtMsgType type, const QString &text,
                 const QString &fileName = QString(),
                 int lineNumber = -1, int columnNumber = -1,
                 const QVariant &data = QVariant());

private:
    QScopedPointer<QScriptStdMessageHandlerPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(QScriptStdMessageHandler)
    Q_DISABLE_COPY(QScriptStdMessageHandler)
};

QT_END_NAMESPACE

#endif
