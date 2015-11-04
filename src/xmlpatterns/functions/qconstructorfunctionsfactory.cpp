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

#include "qatomictype_p.h"
#include "qbuiltintypes_p.h"
#include "qcastas_p.h"
#include "qcommonnamespaces_p.h"
#include "qcommonsequencetypes_p.h"
#include "qfunctionargument_p.h"
#include "qfunctioncall_p.h"
#include "qgenericsequencetype_p.h"
#include "qschematype_p.h"
#include "qschematypefactory_p.h"

#include "qconstructorfunctionsfactory_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

ConstructorFunctionsFactory::ConstructorFunctionsFactory(const NamePool::Ptr &np, const SchemaTypeFactory::Ptr &f) : m_typeFactory(f)
{
    Q_ASSERT(m_typeFactory);
    Q_ASSERT(np);
    SchemaType::Hash::const_iterator it(m_typeFactory->types().constBegin());
    const SchemaType::Hash::const_iterator end(m_typeFactory->types().constEnd());

    FunctionArgument::List args;
    const QXmlName argName(StandardNamespaces::empty, StandardLocalNames::sourceValue);

    args.append(FunctionArgument::Ptr(new FunctionArgument(argName,
                                                           CommonSequenceTypes::ZeroOrOneAtomicType)));

    while(it != end)
    {
        if(!BuiltinTypes::xsAnyAtomicType->wxsTypeMatches(*it) ||
           *BuiltinTypes::xsAnyAtomicType == *static_cast<const AtomicType *>((*it).data()) ||
           *BuiltinTypes::xsNOTATION == *static_cast<const AtomicType *>((*it).data()))
        {
            /* It's not a valid type for a constructor function -- skip it. */
            ++it;
            continue;
        }

        const QXmlName name((*it)->name(np));
        FunctionSignature::Ptr s(new FunctionSignature(name, 1, 1,
                                                       makeGenericSequenceType(AtomicType::Ptr(*it),
                                                                               Cardinality::zeroOrOne())));
        s->setArguments(args);
        m_signatures.insert(name, s);
        ++it;
    }
}

Expression::Ptr ConstructorFunctionsFactory::retrieveExpression(const QXmlName name,
                                                                const Expression::List &args,
                                                                const FunctionSignature::Ptr &sign) const
{
    Q_UNUSED(sign);

    /* This function is only called if the callsite is valid, so createSchemaType() will always
     * return an AtomicType. */
    const AtomicType::Ptr at(static_cast<AtomicType *>(m_typeFactory->createSchemaType(name).data()));

    return Expression::Ptr(new CastAs(args.first(),
                                      makeGenericSequenceType(at,
                                                              Cardinality::zeroOrOne())));
}

FunctionSignature::Ptr ConstructorFunctionsFactory::retrieveFunctionSignature(const NamePool::Ptr &np, const QXmlName name)
{
    Q_UNUSED(np);
    return functionSignatures().value(name);
}

QT_END_NAMESPACE
