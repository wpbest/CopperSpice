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

#include "qatomiccomparator_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

AtomicComparator::AtomicComparator()
{
}

AtomicComparator::~AtomicComparator()
{
}

AtomicComparator::ComparisonResult
AtomicComparator::compare(const Item &,
                          const AtomicComparator::Operator,
                          const Item &) const
{
    Q_ASSERT_X(false, Q_FUNC_INFO, "This function should never be called.");
    return LessThan;
}

QString AtomicComparator::displayName(const AtomicComparator::Operator op,
                                      const ComparisonType type)
{
    Q_ASSERT(type == AsGeneralComparison || type == AsValueComparison);
    if(type == AsGeneralComparison)
    {
        switch(op)
        {
            case OperatorEqual:
                return QLatin1String("=");
            case OperatorGreaterOrEqual:
                return QLatin1String("<=");
            case OperatorGreaterThan:
                return QLatin1String("<");
            case OperatorLessOrEqual:
                return QLatin1String(">=");
            case OperatorLessThanNaNLeast:
            /* Fallthrough. */
            case OperatorLessThanNaNGreatest:
            /* Fallthrough. */
            case OperatorLessThan:
                return QLatin1String(">");
            case OperatorNotEqual:
                return QLatin1String("!=");
        }
    }

    switch(op)
    {
        case OperatorEqual:
            return QLatin1String("eq");
        case OperatorGreaterOrEqual:
            return QLatin1String("ge");
        case OperatorGreaterThan:
            return QLatin1String("gt");
        case OperatorLessOrEqual:
            return QLatin1String("le");
        case OperatorLessThanNaNLeast:
        /* Fallthrough. */
        case OperatorLessThanNaNGreatest:
        /* Fallthrough. */
        case OperatorLessThan:
            return QLatin1String("lt");
        case OperatorNotEqual:
            return QLatin1String("ne");
    }

    Q_ASSERT(false);
    return QString(); /* GCC unbarfer. */
}

QT_END_NAMESPACE
