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

#ifndef QDECLARATIVECLEANUP_P_H
#define QDECLARATIVECLEANUP_P_H

#include <QtCore/qglobal.h>

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

QT_BEGIN_NAMESPACE

class QDeclarativeEngine;
class QDeclarativeCleanup
{
public:
    QDeclarativeCleanup(QDeclarativeEngine *);
    virtual ~QDeclarativeCleanup();

protected:
    virtual void clear() = 0;

private:
    friend class QDeclarativeEnginePrivate;
    QDeclarativeCleanup **prev;
    QDeclarativeCleanup  *next;
};

QT_END_NAMESPACE

#endif // QDECLARATIVECLEANUP_P_H

