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

#ifndef Patternist_ForClause_H
#define Patternist_ForClause_H

#include "qpaircontainer_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    class ForClause : public PairContainer
    {
    public:
        /**
         * If @p positionSlot is -1, no positional variable will be used.
         */
        ForClause(const VariableSlotID varSlot,
                  const Expression::Ptr &bindingSequence,
                  const Expression::Ptr &returnExpression,
                  const VariableSlotID positionSlot);

        virtual Item::Iterator::Ptr evaluateSequence(const DynamicContext::Ptr &context) const;
        virtual Item evaluateSingleton(const DynamicContext::Ptr &context) const;
        virtual void evaluateToSequenceReceiver(const DynamicContext::Ptr &context) const;

        virtual SequenceType::Ptr staticType() const;
        virtual SequenceType::List expectedOperandTypes() const;
        virtual Expression::Ptr typeCheck(const StaticContext::Ptr &context,
                                          const SequenceType::Ptr &reqType);
        virtual ExpressionVisitorResult::Ptr accept(const ExpressionVisitor::Ptr &visitor) const;
        virtual QList<QExplicitlySharedDataPointer<OptimizationPass> > optimizationPasses() const;

        inline Item mapToItem(const Item &item,
                              const DynamicContext::Ptr &context) const;
        virtual ID id() const;
        inline Item::Iterator::Ptr mapToSequence(const Item &item,
                                                 const DynamicContext::Ptr &context) const;

        /**
         * Sets m_allowsMany properly.
         */
        virtual Expression::Ptr compress(const StaticContext::Ptr &context);

    private:
        inline void riggPositionalVariable(const DynamicContext::Ptr &context,
                                           const Item::Iterator::Ptr &source) const;
        typedef QExplicitlySharedDataPointer<const ForClause> ConstPtr;
        const VariableSlotID m_varSlot;
        const VariableSlotID m_positionSlot;
        /**
         * Initialized to @c false. This default is always safe.
         */
        bool m_allowsMany;
    };
}

QT_END_NAMESPACE

#endif
