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

#ifndef Patternist_StackContextBase_P_H
#define Patternist_StackContextBase_P_H

#include <QVector>

#include "qdaytimeduration_p.h"
#include "qdelegatingdynamiccontext_p.h"
#include "qexpression_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{   
    template<typename TSuperClass>
    class StackContextBase : public TSuperClass
    {
    public:
        StackContextBase();
        /**
         * Construct a StackContextBase and passes @p prevContext to its super class. This
         * constructor is typically used when the super class is DelegatingDynamicContext.
         */
        StackContextBase(const DynamicContext::Ptr &prevContext);

        virtual void setRangeVariable(const VariableSlotID slotNumber,
                                      const Item &newValue);
        virtual Item rangeVariable(const VariableSlotID slotNumber) const;

        virtual void setExpressionVariable(const VariableSlotID slotNumber,
                                           const Expression::Ptr &newValue);
        virtual Expression::Ptr expressionVariable(const VariableSlotID slotNumber) const;

        virtual Item::Iterator::Ptr positionIterator(const VariableSlotID slot) const;
        virtual void setPositionIterator(const VariableSlotID slot,
                                         const Item::Iterator::Ptr &newValue);
        virtual ItemCacheCell &itemCacheCell(const VariableSlotID slot);
        virtual ItemSequenceCacheCell::Vector &itemSequenceCacheCells(const VariableSlotID slot);

        virtual DynamicContext::TemplateParameterHash &templateParameterStore();

    protected:
        /**
         * This function is protected, although it only is used in this class. I don't
         * know why it has to be, but it won't compile when private.
         */
        template<typename VectorType, typename UnitType>
        inline
        void setSlotVariable(const VariableSlotID slot,
                             const UnitType &newValue,
                             VectorType &container) const;

    private:
        Item::Vector                            m_rangeVariables;
        Expression::Vector                      m_expressionVariables;
        Item::Iterator::Vector                  m_positionIterators;
        ItemCacheCell::Vector                   m_itemCacheCells;
        ItemSequenceCacheCell::Vector           m_itemSequenceCacheCells;
        DynamicContext::TemplateParameterHash   m_templateParameterStore;
    };

    #include "qstackcontextbase.cpp"

    /**
     * @short A DynamicContext that creates a new scope for variables.
     *
     * This DynamicContext is used for recursive user function calls, for example.
     */
    typedef StackContextBase<DelegatingDynamicContext> StackContext;
}

QT_END_NAMESPACE

#endif