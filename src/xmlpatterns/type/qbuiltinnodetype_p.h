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

#ifndef Patternist_BuiltinNodeType_P_H
#define Patternist_BuiltinNodeType_P_H

#include "qitem_p.h"
#include "qanynodetype_p.h"
#include "qbuiltintypes_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{

    template <const QXmlNodeModelIndex::NodeKind kind>
    class BuiltinNodeType : public AnyNodeType
    {
    public:
        virtual bool xdtTypeMatches(const ItemType::Ptr &other) const;
        virtual bool itemMatches(const Item &item) const;

        /**
         * @returns for example "text()", depending on what the constructor was passed
         */
        virtual QString displayName(const NamePool::Ptr &np) const;

        virtual ItemType::Ptr xdtSuperType() const;
        virtual ItemType::Ptr atomizedType() const;

        QXmlNodeModelIndex::NodeKind nodeKind() const;

        PatternPriority patternPriority() const;

    protected:
        friend class BuiltinTypes;

        /**
         * This constructor does nothing, but exists in order to make it impossible to
         * instantiate this class from anywhere but from BuiltinTypes.
         */
        BuiltinNodeType();
    };

#include "qbuiltinnodetype.cpp"

}

QT_END_NAMESPACE

#endif
