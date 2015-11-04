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

#ifndef Patternist_SequenceType_P_H
#define Patternist_SequenceType_P_H

template<typename T> class QList;

#include <QSharedData>
#include "qcardinality_p.h"
#include "qitemtype_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    class ItemType;
    
    class SequenceType : public virtual QSharedData
    {
    public:
        inline SequenceType()
        {
        }

        /**
         * A smart pointer wrapping SequenceType instances.
         */
        typedef QExplicitlySharedDataPointer<const SequenceType> Ptr;

        /**
         * A list of SequenceType instances, each wrapped in a smart pointer.
         */
        typedef QList<SequenceType::Ptr> List;

        virtual ~SequenceType();

        /**
         * Generates a name for the sequence type for display purposes. The
         * prefix used for the QName identifying the schema type is conventional.
         * An example of a display name for a SequenceType is "xs:integer?".
         */
        virtual QString displayName(const NamePool::Ptr &np) const = 0;

        virtual Cardinality cardinality() const = 0;

        virtual ItemType::Ptr itemType() const = 0;

        /**
         * Determines whether @p other is identical to, or a sub-type
         * of this SequenceType. For example, if this SequenceType is
         * <tt>xs:anyAtomicType</tt>, @c false is returned if @p other is <tt>element()</tt>,
         * but @c true if @p other is <tt>xs:string</tt>.
         *
         * The return values of cardinality() and itemType() used with ItemType::xdtTypeMatches
         * and Cardinality::isWithinScope() is used for achieving this.
         *
         * @see <a href="http://www.w3.org/TR/xquery/#id-sequencetype-matching">XQuery 1.0:
         * An XML Query Language, 2.5.4 SequenceType Matching</a>
         */
        bool matches(const SequenceType::Ptr other) const;

        bool is(const SequenceType::Ptr &other) const;
    private:
        Q_DISABLE_COPY(SequenceType)
    };
}

Q_DECLARE_TYPEINFO(QPatternist::SequenceType::Ptr, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

#endif
