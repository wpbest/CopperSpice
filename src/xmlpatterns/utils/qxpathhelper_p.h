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

#ifndef Patternist_XPathHelper_P_H
#define Patternist_XPathHelper_P_H

#include "qcommonnamespaces_p.h"
#include "qitem_p.h"
#include "qpatternistlocale_p.h"
#include "qreportcontext_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{   
    class XPathHelper
    {
    public:
        /**
         * Determines whether @p qName is a valid QName. For example, "body" and "xhtml:body"
         * is, but "xhtml::body" or "x:body "(note the whitespace) is not.
         *
         * @see QNameConstructor::expandQName()
         * @see QNameValue
         */
        static bool isQName(const QString &qName);

        /**
         * @short Splits @p qName into @p localName and @p prefix.
         *
         * @note @p qName must be a valid QName, and that is not checked.
         */
        static void splitQName(const QString &qName, QString &prefix, QString &localName);

        /**
         * Determines whether @p ns is a reserved namespace.
         *
         * @see <a href="http://www.w3.org/TR/xslt20/#reserved-namespaces">XSL Transformations
         * (XSLT) Version 2.0, 3.2 Reserved Namespaces</a>
         * @see <a href="http://www.w3.org/TR/xquery/#FunctionDeclns">XQuery 1.0: An XML
         * Query Language, 4.15 Function Declaration</a>
         * @returns @c true if @p ns is a reserved namespace, otherwise @c false.
         */
        static bool isReservedNamespace(const QXmlName::NamespaceCode ns);

        /**
         * Determines whether @p collation is a supported string collation. If it is
         * not, error code @p code is raised via @p context.
         */
        template<const ReportContext::ErrorCode code, typename TReportContext>
        static inline void checkCollationSupport(const QString &collation,
                                                 const TReportContext &context,
                                                 const SourceLocationReflection *const r)
        {
            Q_ASSERT(context);
            Q_ASSERT(r);

            if(collation != QLatin1String(CommonNamespaces::UNICODE_COLLATION))
            {
                context->error(QtXmlPatterns::tr("Only the Unicode Codepoint "
                                  "Collation is supported(%1). %2 is unsupported.")
                                  .arg(formatURI(QLatin1String(CommonNamespaces::UNICODE_COLLATION)))
                                  .arg(formatURI(collation)),
                               code, r);
            }
        }

        static QPatternist::ItemTypePtr typeFromKind(const QXmlNodeModelIndex::NodeKind nodeKind);

        /**
         * Normalizes an @p uri by resolving it to the application directory if empty.
         */
        static QUrl normalizeQueryURI(const QUrl &uri);

        /**
         * @short Determines whether @p consists only of whitespace. Characters
         * considered whitespace are the ones for which QChar::isSpace() returns @c true for.
         *
         * For the empty string, @c true is returned.
         *
         * @returns @c true if @p string consists only of whitespace, otherwise @c false.
         */
        static inline bool isWhitespaceOnly(const QStringRef &string)
        {
            const int len = string.length();

            for(int i = 0; i < len; ++i)
            {
                if(!string.at(i).isSpace()) // TODO and this is wrong, see sdk/TODO
                    return false;
            }

            return true;
        }

        /**
         * @overload
         */
        static inline bool isWhitespaceOnly(const QString &string)
        {
            return isWhitespaceOnly(QStringRef(&string));
        }

    private:
        /**
         * @short This default constructor has no definition, in order to avoid
         * instantiation, since it makes no sense to instantiate this class.
         */
        inline XPathHelper();

        Q_DISABLE_COPY(XPathHelper)
    };
}

QT_END_NAMESPACE

#endif
