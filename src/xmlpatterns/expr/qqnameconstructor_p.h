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

#ifndef Patternist_QNameConstructor_H
#define Patternist_QNameConstructor_H

#include "qsinglecontainer_p.h"
#include "qbuiltintypes_p.h"
#include "qpatternistlocale_p.h"
#include "qxpathhelper_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    class QNameConstructor : public SingleContainer
    {
    public:

        QNameConstructor(const Expression::Ptr &source,
                         const NamespaceResolver::Ptr &nsResolver);

        virtual Item evaluateSingleton(const DynamicContext::Ptr &) const;

        virtual SequenceType::List expectedOperandTypes() const;

        virtual SequenceType::Ptr staticType() const;

        virtual ExpressionVisitorResult::Ptr accept(const ExpressionVisitor::Ptr &visitor) const;

        /**
         * Expands @p lexicalQName, which is a lexical representation of a QName such as "x:body", into
         * a QName using @p nsResolver to supply the namespace bindings.
         *
         * If @p lexicalQName is lexically invalid @p InvalidQName is raised via @p context, or if
         * no namespace binding does not exists for a prefix(if any) in @p lexicalQName, @p NoBinding
         * is raised via @p context.
         *
         * If @p asForAttribute is @c true, the name is considered to be for an
         * attribute in some way, and @p lexicalQName will not pick up the
         * default namespace if it doesn't have a prefix.
         *
         * @p nsResolver is parameterized meaning the function can be instantiated with either
         * DynamicContext or StaticContext.
         *
         * @see QQNameValue
         * @see QXmlUtils
         */
        template<typename TReportContext,
                 const ReportContext::ErrorCode InvalidQName,
                 const ReportContext::ErrorCode NoBinding>
        static
        QXmlName expandQName(const QString &lexicalQName,
                             const TReportContext &context,
                             const NamespaceResolver::Ptr &nsResolver,
                             const SourceLocationReflection *const r,
                             const bool asForAttribute = false);

        /**
         * Resolves the namespace prefix @p prefix to its namespace if it exists, or
         * raised ReportContext::XPST0081 otherwise.
         *
         * @returns the namespace URI corresponding to @p prefix
         */
        static QXmlName::NamespaceCode namespaceForPrefix(const QXmlName::PrefixCode prefix,
                                                          const StaticContext::Ptr &context,
                                                          const SourceLocationReflection *const r);

        virtual const SourceLocationReflection *actualReflection() const;

    private:
        const NamespaceResolver::Ptr m_nsResolver;
    };

    template<typename TReportContext,
             const ReportContext::ErrorCode InvalidQName,
             const ReportContext::ErrorCode NoBinding>
    QXmlName QNameConstructor::expandQName(const QString &lexicalQName,
                                           const TReportContext &context,
                                           const NamespaceResolver::Ptr &nsResolver,
                                           const SourceLocationReflection *const r,
                                           const bool asForAttribute)
    {
        Q_ASSERT(nsResolver);
        Q_ASSERT(context);

        if(XPathHelper::isQName(lexicalQName))
        {
            QString prefix;
            QString local;
            XPathHelper::splitQName(lexicalQName, prefix, local);
            const QXmlName::NamespaceCode nsCode = asForAttribute && prefix.isEmpty() ? QXmlName::NamespaceCode(StandardNamespaces::empty)
                                    : (nsResolver->lookupNamespaceURI(context->namePool()->allocatePrefix(prefix)));

            if(nsCode == NamespaceResolver::NoBinding)
            {
                context->error(QtXmlPatterns::tr("No namespace binding exists for "
                           "the prefix %1 in %2").arg(formatKeyword(prefix),formatKeyword(lexicalQName)),NoBinding, r);

                return QXmlName(); /* Silence compiler warning. */
            }
            else
                return context->namePool()->allocateQName(context->namePool()->stringForNamespace(nsCode), local, prefix);
        }
        else
        {
            context->error(QtXmlPatterns::tr("%1 is an invalid %2")
                              .arg(formatData(lexicalQName)).arg(formatType(context->namePool(), BuiltinTypes::xsQName)),InvalidQName, r);

            return QXmlName(); /* Silence compiler warning. */
        }
    }
}

QT_END_NAMESPACE

#endif
