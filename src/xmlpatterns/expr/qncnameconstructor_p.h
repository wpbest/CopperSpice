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

#ifndef QNCNameConstructor_P_H
#define QNCNameConstructor_P_H

#include "qsinglecontainer_p.h"
#include "qpatternistlocale_p.h"
#include "qxmlutils_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    class NCNameConstructor : public SingleContainer
    {
    public:

        NCNameConstructor(const Expression::Ptr &source);

        virtual Item evaluateSingleton(const DynamicContext::Ptr &) const;

        virtual SequenceType::List expectedOperandTypes() const;

        virtual Expression::Ptr typeCheck(const StaticContext::Ptr &context,
                                          const SequenceType::Ptr &reqType);

        virtual SequenceType::Ptr staticType() const;

        virtual ExpressionVisitorResult::Ptr accept(const ExpressionVisitor::Ptr &visitor) const;

        /**
         *  Validates @p lexicalNCName as a processing instruction's target
         *  name, and raise an error if it's not an @c  NCName.
         */
        template<typename TReportContext,
                 const ReportContext::ErrorCode NameIsXML,
                 const ReportContext::ErrorCode LexicallyInvalid>
        static inline
        void validateTargetName(const QString &lexicalNCName,
                                const TReportContext &context,
                                const SourceLocationReflection *const r);
    private:

        /**
         * This translation string is put here in order to avoid duplicate messages and
         * hence reduce work for translators and increase consistency.
         */
        static
        const QString nameIsXML(const QString &lexTarget)
        {
            return QtXmlPatterns::tr("The target name in a processing instruction "
                                     "cannot be %1 in any combination of upper "
                                     "and lower case. Therefore, %2 is invalid.")
                .arg(formatKeyword("xml"), formatKeyword(lexTarget));
        }
    };

    template<typename TReportContext,
             const ReportContext::ErrorCode NameIsXML,
             const ReportContext::ErrorCode LexicallyInvalid>
    inline
    void NCNameConstructor::validateTargetName(const QString &lexicalTarget,
                                               const TReportContext &context,
                                               const SourceLocationReflection *const r)
    {
        Q_ASSERT(context);

        if(QXmlUtils::isNCName(lexicalTarget))
        {
            if(QString::compare(QLatin1String("xml"), lexicalTarget, Qt::CaseInsensitive) == 0)
                context->error(nameIsXML(lexicalTarget), NameIsXML, r);
        }
        else
        {
            context->error(QtXmlPatterns::tr("%1 is not a valid target name in "
                                             "a processing instruction. It "
                                             "must be a %2 value, e.g. %3.")
                           .arg(formatKeyword(lexicalTarget))
                           .arg(formatType(context->namePool(),
                                           BuiltinTypes::xsNCName))
                           .arg(formatKeyword("my-name.123")),
                           LexicallyInvalid,
                           r);
        }
    }
}

QT_END_NAMESPACE

#endif
