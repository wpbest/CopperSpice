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

#ifndef Patternist_AnyURI_P_H
#define Patternist_AnyURI_P_H

#include <QUrl>
#include <QtDebug>

#include "qatomicstring_p.h"
#include "qbuiltintypes_p.h"
#include "qpatternistlocale_p.h"
#include "qreportcontext_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
 
    class AnyURI : public AtomicString
    {
    public:
        typedef QExplicitlySharedDataPointer<AnyURI> Ptr;

        /**
         * Creates an instance representing @p value.
         *
         * @note @p value must be a valid @c xs:anyURI. If it is of interest
         * to construct from a lexical representation, use fromLexical().
         */
        static AnyURI::Ptr fromValue(const QString &value);

        static AnyURI::Ptr fromValue(const QUrl &uri);

        /**
         * @short Treates @p value as a lexical representation of @c xs:anyURI
         * but returns the value instance as a QUrl.
         *
         * If @p value is not a valid lexical representation of @c xs:anyURI,
         * an error is issued via @p context.
         *
         * If @p isValid is passed, no error is raised and it is instead set
         * appropriately.
         */
        template<const ReportContext::ErrorCode code, typename TReportContext>
        static inline QUrl toQUrl(const QString &value,
                                  const TReportContext &context,
                                  const SourceLocationReflection *const r,
                                  bool *const isValid = 0,
                                  const bool issueError = true)
        {
            /* QUrl doesn't flag ":/..." so we workaround it. */
            const QString simplified(value.simplified());
            const QUrl uri(simplified, QUrl::StrictMode);

            if(uri.isEmpty() || (uri.isValid() && (!simplified.startsWith(QLatin1Char(':')) || !uri.isRelative())))
            {
                if(isValid)
                    *isValid = true;

                return uri;
            }
            else
            {
                if(isValid)
                    *isValid = false;

                if(issueError)
                {
                    context->error(QtXmlPatterns::tr("%1 is not a valid value of type %2.").arg(formatURI(value), formatType(context->namePool(), BuiltinTypes::xsAnyURI)),
                                   code, r);
                }

                return QUrl();
            }
        }

        /**
         * @short Return @c true if @p candidate is a valid @c xs:anyURI,
         * otherwise @c false.
         */
        static bool isValid(const QString &candidate);

        /**
         * @short Constructs a @c xs:anyURI value from the lexical representation @p value.
         *
         * If @p value is not a valid lexical representation of @c xs:anyURI,
         * an error is issued via @p context.
         */
        template<const ReportContext::ErrorCode code, typename TReportContext>
        static inline AnyURI::Ptr fromLexical(const QString &value,
                                              const TReportContext &context,
                                              const SourceLocationReflection *const r)
        {
            return AnyURI::Ptr(new AnyURI(toQUrl<code>(value, context, r).toString()));
        }

        /**
         * If @p value is not a valid lexical representation for @c xs:anyURI,
         * a ValidationError is returned.
         */
        static AnyURI::Ptr fromLexical(const QString &value);

        /**
         * Creates an AnyURI instance representing an absolute URI which
         * is created from resolving @p relative against @p base.
         *
         * This function must be compatible with the resolution semantics
         * specified for fn:resolve-uri. In fact, the implementation of fn:resolve-uri,
         * ResourceURIFN, relies on this function.
         *
         * @see <a href="http://www.faqs.org/rfcs/rfc3986.html">RFC 3986 - Uniform
         * Resource Identifier (URI): Generic Syntax</a>
         * @see <a href ="http://www.w3.org/TR/xpath-functions/#func-resolve-uri">XQuery 1.0
         * and XPath 2.0 Functions and Operators, 8.1 fn:resolve-uri</a>
         */
        static AnyURI::Ptr resolveURI(const QString &relative,
                                      const QString &base);

        virtual ItemType::Ptr type() const;

        /**
         * @short Returns this @c xs:anyURI value in a QUrl.
         */
        inline QUrl toQUrl() const
        {
            Q_ASSERT_X(QUrl(m_value).isValid(), Q_FUNC_INFO,
                       qPrintable(QString::fromLatin1("%1 is apparently not ok for QUrl.").arg(m_value)));
            return QUrl(m_value);
        }
    protected:
        friend class CommonValues;

        AnyURI(const QString &value);
    };

    /**
     * @short Formats @p uri, that's considered to be a URI, for display.
     */
    static inline QString formatURI(const NamePool::Ptr &np, const QXmlName::NamespaceCode &uri)
    {
        return formatURI(np->stringForNamespace(uri));
    }
}

QT_END_NAMESPACE

#endif
