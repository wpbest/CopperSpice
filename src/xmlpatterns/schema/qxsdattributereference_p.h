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

#ifndef Patternist_XsdAttributeReference_H
#define Patternist_XsdAttributeReference_H

#include "qxsdattributeuse_p.h"
#include <QtXmlPatterns/QSourceLocation>

QT_BEGIN_NAMESPACE

namespace QPatternist
{
 
    class XsdAttributeReference : public XsdAttributeUse
    {
        public:
            typedef QExplicitlySharedDataPointer<XsdAttributeReference> Ptr;

            /**
             * Describes the type of the attribute reference.
             */
            enum Type
            {
                AttributeUse,   ///< The reference points to an attribute use.
                AttributeGroup  ///< The reference points to an attribute group.
            };

            /**
             * Always returns false, used to avoid dynamic casts.
             */
            virtual bool isAttributeUse() const;

            /**
             * Always returns true, used to avoid dynamic casts.
             */
            virtual bool isReference() const;

            /**
             * Sets the @p type of the attribute reference.
             */
            void setType(Type type);

            /**
             * Returns the type of the attribute reference.
             */
            Type type() const;

            /**
             * Sets the @p name of the attribute or attribute group the
             * attribute reference refers to.
             */
            void setReferenceName(const QXmlName &name);

            /**
             * Returns the name of the attribute or attribute group the
             * attribute reference refers to.
             */
            QXmlName referenceName() const;

            /**
             * Sets the source @p location where the reference is located.
             */
            void setSourceLocation(const QSourceLocation &location);

            /**
             * Returns the source location where the reference is located.
             */
            QSourceLocation sourceLocation() const;

        private:
            Type            m_type;
            QXmlName        m_referenceName;
            QSourceLocation m_sourceLocation;
    };
}

QT_END_NAMESPACE

#endif
