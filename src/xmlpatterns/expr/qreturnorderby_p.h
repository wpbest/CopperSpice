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

#ifndef QReturnOrderBy_P_H
#define QReturnOrderBy_P_H

#include "qorderby_p.h"
#include "qunlimitedcontainer_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    class ReturnOrderBy : public UnlimitedContainer
    {
    public:
        /**
         * In @p operands the first item is the return expression, and the
         * rest, which is at least one, are the sort keys.
         */
        ReturnOrderBy(const OrderBy::Stability stability,
                      const OrderBy::OrderSpec::Vector &oSpecs,
                      const Expression::List &operands);

        virtual Item evaluateSingleton(const DynamicContext::Ptr &context) const;
        virtual bool evaluateEBV(const DynamicContext::Ptr &context) const;
        virtual SequenceType::Ptr staticType() const;
        virtual SequenceType::List expectedOperandTypes() const;
        virtual Expression::Ptr compress(const StaticContext::Ptr &context);
        virtual ExpressionVisitorResult::Ptr accept(const ExpressionVisitor::Ptr &visitor) const;
        virtual ID id() const;

        inline OrderBy::OrderSpec::Vector orderSpecs() const
        {
            return m_orderSpecs;
        }

        inline OrderBy::Stability stability() const
        {
            return m_stability;
        }

        /**
         * In the case of that we don't have a for-expression beloning us, but
         * only a let clause, this ReturnOrderBy breaks if it stays in the AST.
         * So, by default we assume that we should write ourselves away, unless
         * this function is called. The associated ForClause will call it
         * during typeCheck(), if it exists.
         */
        inline void setStay(const bool a)
        {
            m_flyAway = !a;
        }

        virtual Properties properties() const;
    private:
        /**
         * This variable is unfortunately only used at compile time. However,
         * it's tricky to get rid of it due to how QueryTransformParser would
         * have to be adapted.
         */
        const OrderBy::Stability    m_stability;

        OrderBy::OrderSpec::Vector  m_orderSpecs;

        /**
         * @see stay()
         */
        bool                        m_flyAway;
    };
}

QT_END_NAMESPACE

#endif
