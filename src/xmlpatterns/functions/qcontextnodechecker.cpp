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

#include "qcontextnodechecker_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

void ContextNodeChecker::checkTargetNode(const QXmlNodeModelIndex &node,
                                         const DynamicContext::Ptr &context,
                                         const ReportContext::ErrorCode code) const
{
    if(node.root().kind() != QXmlNodeModelIndex::Document)
    {
        context->error(QtXmlPatterns::tr("The root node of the second argument "
                                         "to function %1 must be a document "
                                         "node. %2 is not a document node.")
                       .arg(formatFunction(context->namePool(), signature()),
                            formatData(node)),
                       code, this);
    }
}

QT_END_NAMESPACE
