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
#include "qcommonsequencetypes_p.h"
#include "qitem_p.h"

#include "qebvtype_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

EBVType::EBVType()
{
}

bool EBVType::itemMatches(const Item &item) const
{
    if(item.isNode())
        return false;

    return BuiltinTypes::xsBoolean->itemMatches(item)       ||
           BuiltinTypes::numeric->itemMatches(item)         ||
           BuiltinTypes::xsString->itemMatches(item)        ||
           BuiltinTypes::xsAnyURI->itemMatches(item)        ||
           CommonSequenceTypes::Empty->itemMatches(item)    ||
           BuiltinTypes::xsUntypedAtomic->itemMatches(item);
}

bool EBVType::xdtTypeMatches(const ItemType::Ptr &t) const
{
    return BuiltinTypes::node->xdtTypeMatches(t)            ||
           BuiltinTypes::xsBoolean->xdtTypeMatches(t)       ||
           BuiltinTypes::numeric->xdtTypeMatches(t)         ||
           BuiltinTypes::xsString->xdtTypeMatches(t)        ||
           BuiltinTypes::xsAnyURI->xdtTypeMatches(t)        ||
           *CommonSequenceTypes::Empty == *t                ||
           BuiltinTypes::xsUntypedAtomic->xdtTypeMatches(t) ||
           /* Item & xs:anyAtomicType is ok, we do the checking at runtime. */
           *BuiltinTypes::item == *t                        ||
           *BuiltinTypes::xsAnyAtomicType == *t;
}

QString EBVType::displayName(const NamePool::Ptr &) const
{
    /* Some QName-lexical is not used here because it makes little sense
     * for this strange type. Instead the operand type of the fn:boolean()'s
     * argument is used. */
    return QLatin1String("item()*(: EBV extractable type :)");
}

Cardinality EBVType::cardinality() const
{
    return Cardinality::zeroOrMore();
}

ItemType::Ptr EBVType::xdtSuperType() const
{
    return BuiltinTypes::item;
}

ItemType::Ptr EBVType::itemType() const
{
    return ItemType::Ptr(const_cast<EBVType *>(this));
}

bool EBVType::isAtomicType() const
{
    return false;
}

bool EBVType::isNodeType() const
{
    return true;
}

ItemType::Ptr EBVType::atomizedType() const
{
    Q_ASSERT_X(false, Q_FUNC_INFO,
               "That this function is called makes no sense.");
    return AtomicType::Ptr();
}

QT_END_NAMESPACE
