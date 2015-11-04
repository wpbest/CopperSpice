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

#ifndef Patternist_GenericNamespaceResolver_P_H
#define Patternist_GenericNamespaceResolver_P_H

#include <QHash>
#include "qnamespaceresolver_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{    
    class GenericNamespaceResolver : public NamespaceResolver
    {
    public:
        GenericNamespaceResolver(const Bindings &list);
        virtual void addBinding(const QXmlName nb);

        virtual QXmlName::NamespaceCode lookupNamespaceURI(const QXmlName::PrefixCode prefix) const;

        /**
         * Returns a GenericNamespaceResolver containing the following bindings:
         *
         * - <tt>xml</tt> = <tt>http://www.w3.org/XML/1998/namespace</tt>
         * - <tt>xs</tt> = <tt>http://www.w3.org/2001/XMLSchema</tt>
         * - <tt>xsi</tt> = <tt>http://www.w3.org/2001/XMLSchema-instance</tt>
         * - <tt>fn</tt> = <tt>http://www.w3.org/2005/xpath-functions</tt>
         * - <tt>xdt</tt> = <tt>http://www.w3.org/2005/xpath-datatypes</tt>
         * - no prefix = empty namespace
         */
        static NamespaceResolver::Ptr defaultXQueryBindings();

        /**
         * Returns a GenericNamespaceResolver containing the following bindings:
         *
         * - <tt>xml</tt> = <tt>http://www.w3.org/XML/1998/namespace</tt>
         * - no prefix = empty namespace
         */
        static NamespaceResolver::Ptr defaultXSLTBindings();

        virtual Bindings bindings() const;

    private:
        /**
         * The key is the prefix, the value the namespace URI.
         */
        Bindings m_bindings;
    };
}

QT_END_NAMESPACE

#endif
