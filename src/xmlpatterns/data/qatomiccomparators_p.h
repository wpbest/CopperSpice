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

#ifndef Patternist_AtomicComparators_P_H
#define Patternist_AtomicComparators_P_H

#include "qabstractfloat_p.h"
#include "qatomiccomparator_p.h"
#include "qprimitives_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{

    class StringComparator : public AtomicComparator
    {

    public:

        virtual ComparisonResult compare(const Item &op1,
                                         const AtomicComparator::Operator op,
                                         const Item &op2) const;

        /**
         * Compares two strings, and returns @c true if they are considered equal as per
         * an ordinary string comparison. In other words, this is an implementation with
         * the Unicode code point collation.
         *
         * @see <a href="http://www.w3.org/TR/xpath-functions/#string-compare">XQuery 1.0 and XPath
         * 2.0 Functions and Operators, 7.3 ValueComparison::Equality and Comparison of Strings</a>
         */
        virtual bool equals(const Item &op1,
                            const Item &op2) const;
    };

    class CaseInsensitiveStringComparator : public AtomicComparator
    {
    public:
        /**
         * Converts both string values to upper case and afterwards compare them.
         */
        virtual ComparisonResult compare(const Item &op1,
                                         const AtomicComparator::Operator op,
                                         const Item &op2) const;

        /**
         * Converts both string values case insensitively.
         */
        virtual bool equals(const Item &op1,
                            const Item &op2) const;
    };


    class BinaryDataComparator : public AtomicComparator
    {
    public:
        virtual bool equals(const Item &op1,
                            const Item &op2) const;
    };

    class BooleanComparator : public AtomicComparator
    {
    public:
        virtual ComparisonResult compare(const Item &op1,
                                         const AtomicComparator::Operator op,
                                         const Item &op2) const;

        virtual bool equals(const Item &op1,
                            const Item &op2) const;
    };


    class AbstractFloatComparator : public AtomicComparator
    {
    public:
        virtual ComparisonResult compare(const Item &op1,
                                         const AtomicComparator::Operator op,
                                         const Item &op2) const;

        virtual bool equals(const Item &op1,
                            const Item &op2) const;
    };

    template<const AtomicComparator::Operator t_op>
    class AbstractFloatSortComparator : public AbstractFloatComparator
    {
    public:
        virtual ComparisonResult compare(const Item &o1,
                                         const AtomicComparator::Operator op,
                                         const Item &o2) const
        {
            Q_ASSERT_X(t_op == OperatorLessThanNaNLeast || t_op == OperatorLessThanNaNGreatest, Q_FUNC_INFO,
                       "Can only be instantiated with those two.");
            Q_ASSERT(op == t_op);
            Q_UNUSED(op); /* Needed when building in release mode. */

            const xsDouble v1 = o1.template as<Numeric>()->toDouble();
            const xsDouble v2 = o2.template as<Numeric>()->toDouble();

            if(qIsNaN(v1) && !qIsNaN(v2))
                return t_op == OperatorLessThanNaNLeast ? LessThan : GreaterThan;
            if(!qIsNaN(v1) && qIsNaN(v2))
                return t_op == OperatorLessThanNaNLeast ? GreaterThan : LessThan;

            if(Double::isEqual(v1, v2))
                return Equal;
            else if(v1 < v2)
                return LessThan;
            else
                return GreaterThan;
        }

    };


    class DecimalComparator : public AtomicComparator
    {
    public:
        virtual ComparisonResult compare(const Item &op1,
                                         const AtomicComparator::Operator op,
                                         const Item &op2) const;

        virtual bool equals(const Item &op1,
                            const Item &op2) const;
    };


    class IntegerComparator : public AtomicComparator
    {
    public:
        virtual ComparisonResult compare(const Item &op1,
                                         const AtomicComparator::Operator op,
                                         const Item &op2) const;

        virtual bool equals(const Item &op1,
                            const Item &op2) const;
    };


    class QNameComparator : public AtomicComparator
    {
    public:
        virtual bool equals(const Item &op1,
                            const Item &op2) const;
    };

    class AbstractDateTimeComparator : public AtomicComparator
    {
    public:
        virtual ComparisonResult compare(const Item &op1,
                                         const AtomicComparator::Operator op,
                                         const Item &op2) const;
        virtual bool equals(const Item &op1,
                            const Item &op2) const;
    };


    class AbstractDurationComparator : public AtomicComparator
    {
    public:
        virtual ComparisonResult compare(const Item &op1,
                                         const AtomicComparator::Operator op,
                                         const Item &op2) const;
        virtual bool equals(const Item &op1,
                            const Item &op2) const;

    private:
        static inline QDateTime addDurationToDateTime(const QDateTime &dateTime,
                                                      const AbstractDuration *const duration);
    };
}

QT_END_NAMESPACE

#endif
