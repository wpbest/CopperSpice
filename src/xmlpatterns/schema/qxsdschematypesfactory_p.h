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

#ifndef Patternist_XsdSchemaTypesFactory_H
#define Patternist_XsdSchemaTypesFactory_H

#include <QtCore/QHash>
#include "qschematypefactory_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
   
    class XsdSchemaTypesFactory : public SchemaTypeFactory
    {
        public:
            /**
             * Creates a new schema type factory.
             *
             * @param namePool The name pool all type names belong to.
             */
            XsdSchemaTypesFactory(const NamePool::Ptr &namePool);

            /**
             * Creates a primitive type for @p name. If @p name is not supported,
             * @c null is returned.
             *
             * @note This does not handle user defined types, only builtin types.
             */
            virtual SchemaType::Ptr createSchemaType(const QXmlName) const;

            /**
             * Returns a hash of all available types.
             */
            virtual SchemaType::Hash types() const;

        private:
            /**
             * A dictonary of all predefined schema types.
             */
            SchemaType::Hash               m_types;

            NamePool::Ptr                  m_namePool;
            mutable SchemaTypeFactory::Ptr m_basicTypesFactory;
    };
}

QT_END_NAMESPACE

#endif
