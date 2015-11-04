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

#ifndef PATTERNIST_VARIABLELOADER_P_H
#define PATTERNIST_VARIABLELOADER_P_H

#include <QtCore/QSet>
#include <QtXmlPatterns/QXmlQuery>
#include <QtDebug>

#include "qdynamiccontext_p.h"
#include "qexternalvariableloader_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    class VariableLoader : public ExternalVariableLoader
    {
    public:
        typedef QHash<QXmlName, QVariant> BindingHash;
        typedef QExplicitlySharedDataPointer<VariableLoader> Ptr;

        inline VariableLoader(const NamePool::Ptr &np,
                              const VariableLoader::Ptr &previousLoader = VariableLoader::Ptr()) : m_namePool(np)
                                                                                                 , m_previousLoader(previousLoader)

        {
        }

        virtual QPatternist::SequenceType::Ptr announceExternalVariable(const QXmlName name,
                                                                        const QPatternist::SequenceType::Ptr &declaredType);
        virtual QPatternist::Item::Iterator::Ptr evaluateSequence(const QXmlName name,
                                                                  const QPatternist::DynamicContext::Ptr &);

        virtual QPatternist::Item evaluateSingleton(const QXmlName name,
                                                    const QPatternist::DynamicContext::Ptr &);

        void removeBinding(const QXmlName &name);
        bool hasBinding(const QXmlName &name) const;
        QVariant valueFor(const QXmlName &name) const;
        void addBinding(const QXmlName &name,
                        const QVariant &value);

        bool isSameType(const QVariant &v1,
                        const QVariant &v2) const;

        bool invalidationRequired(const QXmlName &name,
                                  const QVariant &variant) const;

    private:

        inline QPatternist::Item itemForName(const QXmlName &name) const;

        const NamePool::Ptr                 m_namePool;
        VariableLoader::Ptr                 m_previousLoader;
        BindingHash                         m_bindingHash;
    };
}

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QIODevice *)
Q_DECLARE_METATYPE(QXmlQuery)

#endif
