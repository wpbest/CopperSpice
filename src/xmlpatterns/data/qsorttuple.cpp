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

#include <QString>

#include "qbuiltintypes_p.h"
#include "qsorttuple_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

bool SortTuple::isAtomicValue() const
{
    Q_ASSERT_X(false, Q_FUNC_INFO,
               "It makes no sense to call this function.");
    return false;
}

QString SortTuple::stringValue() const
{
    return QLatin1String("SortTuple");
}

bool SortTuple::isNode() const
{
    Q_ASSERT_X(false, Q_FUNC_INFO,
               "It makes no sense to call this function.");
    return false;
}

bool SortTuple::hasError() const
{
    Q_ASSERT_X(false, Q_FUNC_INFO,
               "It makes no sense to call this function.");
    return false;
}

Item::Iterator::Ptr SortTuple::typedValue() const
{
    Q_ASSERT_X(false, Q_FUNC_INFO,
               "It makes no sense to call this function.");
    return Item::Iterator::Ptr();
}

ItemType::Ptr SortTuple::type() const
{
    return BuiltinTypes::xsAnyAtomicType;
}

QT_END_NAMESPACE
