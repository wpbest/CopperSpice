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

#ifndef Patternist_ExpressionVariableReference_H
#define Patternist_ExpressionVariableReference_H

#include "qvariabledeclaration_p.h"
#include "qvariablereference_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    class ExpressionVariableReference : public VariableReference
    {
    public:
        ExpressionVariableReference(const VariableSlotID slot, const VariableDeclaration *varDecl);

        virtual bool evaluateEBV(const DynamicContext::Ptr &context) const;
        virtual Item evaluateSingleton(const DynamicContext::Ptr &context) const;
        virtual Item::Iterator::Ptr evaluateSequence(const DynamicContext::Ptr &context) const;

        virtual Expression::Ptr typeCheck(const StaticContext::Ptr &context, const SequenceType::Ptr &reqType);
        virtual SequenceType::Ptr staticType() const;
        virtual ID id() const;
        virtual ExpressionVisitorResult::Ptr accept(const ExpressionVisitor::Ptr &visitor) const;

        inline const Expression::Ptr &sourceExpression() const;
        inline const VariableDeclaration *variableDeclaration() const;
    private:
        const VariableDeclaration *m_varDecl;
    };

    inline const Expression::Ptr &ExpressionVariableReference::sourceExpression() const
    {
        return m_varDecl->expression();
    }

    inline const VariableDeclaration *ExpressionVariableReference::variableDeclaration() const
    {
        return m_varDecl;
    }

}

QT_END_NAMESPACE

#endif
