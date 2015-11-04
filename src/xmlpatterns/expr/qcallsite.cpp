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

#include "qcallsite_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

CallSite::CallSite(const QXmlName &name) : m_isRecursive(false)
                                         , m_name(name)
{
}

QXmlName CallSite::name() const
{
    return m_name;
}

bool CallSite::isRecursive() const
{
    return m_isRecursive;
}

void CallSite::setIsRecursive(const bool value)
{
    m_isRecursive = value;
}

QT_END_NAMESPACE
