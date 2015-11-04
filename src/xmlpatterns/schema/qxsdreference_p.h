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

#ifndef Patternist_XsdReference_H
#define Patternist_XsdReference_H

#include "qxsdterm_p.h"
#include <QtXmlPatterns/QSourceLocation>

QT_BEGIN_NAMESPACE

namespace QPatternist
{

    class XsdReference : public XsdTerm
    {
        public:
            typedef QExplicitlySharedDataPointer<XsdReference> Ptr;

            /**
             * Describes the type of the reference.
             */
            enum Type
            {
                Element,        ///< The reference points to an element.
                ModelGroup      ///< The reference points to a model group.
            };

            /**
             * Returns always @c true, used to avoid dynamic casts.
             */
            virtual bool isReference() const;

            /**
             * Sets the @p type of the reference.
             *
             * @see Type
             */
            void setType(Type type);

            /**
             * Returns the type of the reference.
             */
            Type type() const;

            /**
             * Sets the @p name of the referenced object.
             *
             * The name can either be a top-level element declaration
             * or a top-level group declaration.
             */
            void setReferenceName(const QXmlName &ame);

            /**
             * Returns the name of the referenced object.
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
