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

#ifndef QDECLARATIVETYPENAMESCRIPTCLASS_P_H
#define QDECLARATIVETYPENAMESCRIPTCLASS_P_H

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
#include "private/qdeclarativeengine_p.h"

#include <private/qscriptdeclarativeclass_p.h>
#include <QtScript/qscriptclass.h>

QT_BEGIN_NAMESPACE

class QDeclarativeEngine;
class QDeclarativeType;
class QDeclarativeTypeNameCache;
class QDeclarativeTypeNameScriptClass : public QScriptDeclarativeClass
{
public:
    QDeclarativeTypeNameScriptClass(QDeclarativeEngine *);
    ~QDeclarativeTypeNameScriptClass();

    enum TypeNameMode { IncludeEnums, ExcludeEnums };
    QScriptValue newObject(QObject *, QDeclarativeType *, TypeNameMode = IncludeEnums);
    QScriptValue newObject(QObject *, QDeclarativeTypeNameCache *, TypeNameMode = IncludeEnums);

protected:
    virtual QScriptClass::QueryFlags queryProperty(Object *, const Identifier &, 
                                                   QScriptClass::QueryFlags flags);

    virtual Value property(Object *, const Identifier &);
    virtual void setProperty(Object *, const Identifier &name, const QScriptValue &);

private:
    QDeclarativeEngine *engine;
    QObject *object;
    QDeclarativeType *type;
    quint32 enumValue;
};

QT_END_NAMESPACE

#endif // QDECLARATIVETYPENAMESCRIPTCLASS_P_H

