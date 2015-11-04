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

#ifndef Patternist_ExternalVariableLoader_H
#define Patternist_ExternalVariableLoader_H

#include "qitem_p.h"
#include "qsequencetype_p.h"
#include "qxmlname.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    class DynamicContext;

     class ExternalVariableLoader : public QSharedData
    {
    public:
        typedef QExplicitlySharedDataPointer<ExternalVariableLoader> Ptr;
        inline ExternalVariableLoader() {}

        virtual ~ExternalVariableLoader();

        virtual SequenceType::Ptr announceExternalVariable(const QXmlName name, const SequenceType::Ptr &declaredType);
        virtual Item::Iterator::Ptr evaluateSequence(const QXmlName name, const QExplicitlySharedDataPointer<DynamicContext> &context);
        virtual Item evaluateSingleton(const QXmlName name, const QExplicitlySharedDataPointer<DynamicContext> &context);
        virtual bool evaluateEBV(const QXmlName name, const QExplicitlySharedDataPointer<DynamicContext> &context);
    };
}

QT_END_NAMESPACE

#endif
