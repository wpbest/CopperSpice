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

#ifndef Patternist_AtomicType_P_H
#define Patternist_AtomicType_P_H

#include "qanysimpletype_p.h"
#include "qatomiccasterlocator_p.h"
#include "qatomiccomparatorlocator_p.h"
#include "qatomicmathematicianlocator_p.h"
#include "qatomictypedispatch_p.h"
#include "qitemtype_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    class Item;
    class SourceLocationReflection;
   
    class AtomicType : public ItemType, public AnySimpleType
    {
    public:

        typedef QExplicitlySharedDataPointer<AtomicType> Ptr;

        virtual ~AtomicType();

        /**
         * Implements a generic algorithm which relies on wxsTypeMatches().
         *
         * @returns @c true depending on if @p item is an atomic type, and that
         * AtomicValue::itemType()'s SequenceType::itemType() matches this type.
         */
        virtual bool itemMatches(const Item &item) const;

        /**
         * @returns the result of SharedQXmlName::displayName(), of the SharedQName
         * object returned from the name() function.
         */
        virtual QString displayName(const NamePool::Ptr &np) const;

        /**
         * returns always @c false
         */
        virtual bool isNodeType() const;

        /**
         * returns always @c true
         */
        virtual bool isAtomicType() const;

        /**
         * Determines whether @p other is equal to this type, or is a
         * sub-type of this type.
         *
         * The implementation is generic, relying on operator==()
         * and xdtSuperType().
         */
        virtual bool xdtTypeMatches(const ItemType::Ptr &other) const;

        /**
         * @returns always 'this'
         */
        virtual ItemType::Ptr atomizedType() const;

        /**
         * @returns always SchemaType::SimpleTypeAtomic
         */
        virtual TypeCategory category() const;

        /**
         * @returns DerivationRestriction
         */
        virtual DerivationMethod derivationMethod() const;

        virtual AtomicTypeVisitorResult::Ptr
        accept(const QExplicitlySharedDataPointer<AtomicTypeVisitor> &visitor,
               const SourceLocationReflection *const) const = 0;

        virtual AtomicTypeVisitorResult::Ptr
        accept(const ParameterizedAtomicTypeVisitor::Ptr &visitor,
               const qint16 param,
               const SourceLocationReflection *const) const = 0;

        virtual AtomicComparatorLocator::Ptr comparatorLocator() const = 0;
        virtual AtomicMathematicianLocator::Ptr mathematicianLocator() const = 0;
        virtual AtomicCasterLocator::Ptr casterLocator() const = 0;

    protected:
        AtomicType();

    };
}

QT_END_NAMESPACE

#endif
