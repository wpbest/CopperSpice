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

#ifndef Patternist_RangeExpression_H
#define Patternist_RangeExpression_H

#include "qpaircontainer_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{

    class RangeExpression : public PairContainer
    {
    public:
        RangeExpression(const Expression::Ptr &operand1, const Expression::Ptr &operand2);

        virtual Item::Iterator::Ptr evaluateSequence(const DynamicContext::Ptr &) const;
        /**
         * It's likely that this function gets called if staticType() inferred
         * the cardinality to an exact number. In that case, we know that the
         * first arguments is the same as the second argument.
         */
        virtual Item evaluateSingleton(const DynamicContext::Ptr &) const;

        virtual SequenceType::List expectedOperandTypes() const;

        /**
         * @returns always CommonSequenceTypes::ZeroOrMoreIntegers
         */
        virtual SequenceType::Ptr staticType() const;

        /**
         * Disables compression for optimization reasons. For example, the
         * expression "1 to 1000" would consume thousand allocated instances
         * of Integer, and RangeIterator is well suited for dynamic evaluation.
         *
         * @returns Expression::DisableElimination
         */
        virtual Expression::Properties properties() const;

        virtual ExpressionVisitorResult::Ptr accept(const ExpressionVisitor::Ptr &visitor) const;
    };
}

QT_END_NAMESPACE

#endif
