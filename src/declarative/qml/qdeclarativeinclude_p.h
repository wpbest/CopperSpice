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

#ifndef QDECLARATIVEINCLUDE_P_H
#define QDECLARATIVEINCLUDE_P_H

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

#include <QtCore/qobject.h>
#include <QtCore/qurl.h>
#include <QtScript/qscriptvalue.h>

#include <private/qdeclarativecontext_p.h>
#include <private/qdeclarativeguard_p.h>

QT_BEGIN_NAMESPACE

class QDeclarativeEngine;
class QScriptContext;
class QScriptEngine;
class QNetworkAccessManager;
class QNetworkReply;
class QDeclarativeInclude : public QObject
{
    CS_OBJECT(QDeclarativeInclude)
public:
    enum Status {
        Ok = 0,
        Loading = 1,
        NetworkError = 2,
        Exception = 3
    };

    QDeclarativeInclude(const QUrl &, QDeclarativeEngine *, QScriptContext *ctxt);
    ~QDeclarativeInclude();

    void setCallback(const QScriptValue &);
    QScriptValue callback() const;

    QScriptValue result() const;

    static QScriptValue resultValue(QScriptEngine *, Status status = Loading);
    static void callback(QScriptEngine *, QScriptValue &callback, QScriptValue &status);

    static QScriptValue include(QScriptContext *ctxt, QScriptEngine *engine);
    static QScriptValue worker_include(QScriptContext *ctxt, QScriptEngine *engine);

public :
    CS_SLOT_1(Public, void finished())
    CS_SLOT_2(finished) 

private:
    QDeclarativeEngine *m_engine;
    QScriptEngine *m_scriptEngine;
    QNetworkAccessManager *m_network;
    QDeclarativeGuard<QNetworkReply> m_reply;

    QUrl m_url;
    int m_redirectCount;
    QScriptValue m_callback;
    QScriptValue m_result;
    QDeclarativeGuardedContextData m_context;
    QScriptValue m_scope[2];
};

QT_END_NAMESPACE

#endif // QDECLARATIVEINCLUDE_P_H

