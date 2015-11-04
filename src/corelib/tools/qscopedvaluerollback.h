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

#ifndef QSCOPEDVALUEROLLBACK_H
#define QSCOPEDVALUEROLLBACK_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

template <typename T>
class QScopedValueRollback
{
public:
    QScopedValueRollback(T &var) :
        varRef(var)
    {
        oldValue = varRef;
    }

    ~QScopedValueRollback()
    {
        varRef = oldValue;
    }

    void commit()
    {
        oldValue = varRef;
    }

private:
    T& varRef;
    T oldValue;

    Q_DISABLE_COPY(QScopedValueRollback)
};

QT_END_NAMESPACE

#endif // QSCOPEDVALUEROLLBACK_H
