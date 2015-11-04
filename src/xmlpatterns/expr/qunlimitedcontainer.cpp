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

#include "qunlimitedcontainer_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

UnlimitedContainer::UnlimitedContainer(const Expression::List &ops) : m_operands(ops)
{
}

void UnlimitedContainer::setOperands(const Expression::List &list)
{
    m_operands = list;
}

Expression::List UnlimitedContainer::operands() const
{
    return m_operands;
}

bool UnlimitedContainer::compressOperands(const StaticContext::Ptr &context)
{
    const Expression::List::iterator end(m_operands.end());
    Expression::List::iterator it(m_operands.begin());
    int evaled = 0;

    for(; it != end; ++it)
    {
        Q_ASSERT((*it));
        rewrite((*it), (*it)->compress(context), context);
        if((*it)->isEvaluated())
                ++evaled;
    }

    return evaled == m_operands.count();
}

QT_END_NAMESPACE
