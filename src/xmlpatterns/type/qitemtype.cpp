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

#include <QtGlobal>

#include "qcommonsequencetypes_p.h"

#include "qitemtype_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

ItemType::~ItemType()
{
}

const ItemType &ItemType::operator|(const ItemType &other) const
{
    const ItemType *ca = this;

    if(other == *CommonSequenceTypes::None)
        return *ca;

    if(*ca == *CommonSequenceTypes::Empty)
        return other;
    else if(other == *CommonSequenceTypes::Empty)
        return *ca;

    do
    {
        const ItemType *cb = &other;
        do
        {
            if(*ca == *cb)
                return *ca;

            cb = cb->xdtSuperType().data();
        }
        while(cb);

        ca = ca->xdtSuperType().data();
    }
    while(ca);

    Q_ASSERT_X(false, Q_FUNC_INFO, "We should never reach this line.");
    return *this;
}

ItemType::Category ItemType::itemTypeCategory() const
{
    return Other;
}

bool ItemType::operator==(const ItemType &other) const
{
    return this == &other;
}

ItemType::InstanceOf ItemType::instanceOf() const
{
    return ClassOther;
}

QT_END_NAMESPACE
