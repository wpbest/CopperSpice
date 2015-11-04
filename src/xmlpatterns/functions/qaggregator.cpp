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

#include "qbuiltintypes_p.h"
#include "qgenericsequencetype_p.h"

#include "qaggregator_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

SequenceType::Ptr Aggregator::staticType() const
{
    const SequenceType::Ptr t(m_operands.first()->staticType());
    ItemType::Ptr itemType(t->itemType());

    /* Since we have types that are derived from xs:integer, this ensures that
     * the static type is xs:integer even if the argument is for
     * instance xs:unsignedShort. */
    if(BuiltinTypes::xsInteger->xdtTypeMatches(itemType) &&
       !itemType->xdtTypeMatches(BuiltinTypes::xsInteger))
    {
        itemType = BuiltinTypes::xsInteger;
    }

    return makeGenericSequenceType(itemType,
                                   t->cardinality().toWithoutMany());
}

QT_END_NAMESPACE