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

#include "quntyped_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

Untyped::Untyped()
{
}

SchemaType::Ptr Untyped::wxsSuperType() const
{
    return BuiltinTypes::xsAnyType;
}

QXmlName Untyped::name(const NamePool::Ptr &np) const
{
    return np->allocateQName(StandardNamespaces::xs, QLatin1String("untyped"));
}

ItemType::Ptr Untyped::atomizedType() const
{
    return BuiltinTypes::xsUntypedAtomic;
}

SchemaType::TypeCategory Untyped::category() const
{
    return SchemaType::ComplexType;
}

SchemaType::DerivationMethod Untyped::derivationMethod() const
{
    return NoDerivation;
}

QT_END_NAMESPACE
