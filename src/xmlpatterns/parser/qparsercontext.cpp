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

#include <QtGlobal>

#include "qexpression_p.h"
#include "qstaticcontext_p.h"
#include "qtokenizer_p.h"

#include "qparsercontext_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

ParserContext::ParserContext(const StaticContext::Ptr &context,
                             const QXmlQuery::QueryLanguage lang,
                             Tokenizer *const tokener) : staticContext(context)
                                                       , tokenizer(tokener)
                                                       , languageAccent(lang)
                                                       , nodeTestSource(BuiltinTypes::element)
                                                       , moduleNamespace(StandardNamespaces::empty)
                                                       , isPreviousEnclosedExpr(false)
                                                       , elementConstructorDepth(0)
                                                       , hasSecondPrologPart(false)
                                                       , preserveNamespacesMode(true)
                                                       , inheritNamespacesMode(true)
                                                       , isParsingPattern(false)
                                                       , currentImportPrecedence(1)
                                                       , m_evaluationCacheSlot(-1)
                                                       , m_expressionSlot(0)
                                                       , m_positionSlot(-1)
                                                       , m_globalVariableSlot(-1)
                                                       , m_currentTemplateID(InitialTemplateID)
{
    resolvers.push(context->namespaceBindings());
    Q_ASSERT(tokenizer);
    Q_ASSERT(context);
    m_isParsingWithParam.push(false);
    isBackwardsCompat.push(false);
}

void ParserContext::finalizePushedVariable(const int amount,
                                           const bool shouldPop)
{
    for(int i = 0; i < amount; ++i)
    {
        const VariableDeclaration::Ptr var(shouldPop ? variables.pop() : variables.top());
        Q_ASSERT(var);

        if(var->isUsed())
            continue;
        else
        {
            staticContext->warning(QtXmlPatterns::tr("The variable %1 is unused")
                                                     .arg(formatKeyword(var, staticContext->namePool())));
        }
    }
}

QT_END_NAMESPACE

