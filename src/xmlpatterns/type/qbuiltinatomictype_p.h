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

#ifndef Patternist_BuiltinAtomicType_P_H
#define Patternist_BuiltinAtomicType_P_H

#include "qatomictype_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{   
    class BuiltinAtomicType : public AtomicType
    {
    public:

        typedef QExplicitlySharedDataPointer<BuiltinAtomicType> Ptr;

        /**
         * @returns always @c false
         */
        virtual bool isAbstract() const;

        /**
         * @returns the base type as specified in the constructors baseType argument.
         */
        virtual SchemaType::Ptr wxsSuperType() const;

        /**
         * @returns the same type as wxsSuperType(), except for the type @c xs:anyAtomicType, which
         * returns item()
         */
        virtual ItemType::Ptr xdtSuperType() const;

        virtual AtomicComparatorLocator::Ptr comparatorLocator() const;
        virtual AtomicMathematicianLocator::Ptr mathematicianLocator() const;
        virtual AtomicCasterLocator::Ptr casterLocator() const;

    protected:
        friend class BuiltinTypes;

        /**
         * @param baseType the type that is the super type of the constructed
         * atomic type. In the case of AnyAtomicType, @c null is passed.
         * @param comp the AtomicComparatorLocator this type should return. May be @c null.
         * @param mather similar to @p comp, this is the AtomicMathematicianLocator
         * that's appropriate for this type May be @c null.
         * @param casterLocator the CasterLocator that locates classes performing
         * casting with this type. May be @c null.
         */
        BuiltinAtomicType(const AtomicType::Ptr &baseType,
                          const AtomicComparatorLocator::Ptr &comp,
                          const AtomicMathematicianLocator::Ptr &mather,
                          const AtomicCasterLocator::Ptr &casterLocator);

    private:
        const AtomicType::Ptr                   m_superType;
        const AtomicComparatorLocator::Ptr      m_comparatorLocator;
        const AtomicMathematicianLocator::Ptr   m_mathematicianLocator;
        const AtomicCasterLocator::Ptr          m_casterLocator;
    };
}

QT_END_NAMESPACE

#endif