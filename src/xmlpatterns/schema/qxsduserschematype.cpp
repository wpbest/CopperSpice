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

/*
 * NOTE: This file is included by qxsduserschematype_p.h
 * if you need some includes, put them in qxsduserschematype_p.h (outside of the namespace)
 */

template<typename TSuperClass>
void XsdUserSchemaType<TSuperClass>::setName(const QXmlName &name)
{
    m_name = name;
}

template<typename TSuperClass>
QXmlName XsdUserSchemaType<TSuperClass>::name(const NamePool::Ptr&) const
{
    return m_name;
}

template<typename TSuperClass>
QString XsdUserSchemaType<TSuperClass>::displayName(const NamePool::Ptr &np) const
{
    return np->displayName(m_name);
}

template<typename TSuperClass>
void XsdUserSchemaType<TSuperClass>::setDerivationConstraints(const SchemaType::DerivationConstraints &constraints)
{
    m_derivationConstraints = constraints;
}

template<typename TSuperClass>
SchemaType::DerivationConstraints XsdUserSchemaType<TSuperClass>::derivationConstraints() const
{
    return m_derivationConstraints;
}
