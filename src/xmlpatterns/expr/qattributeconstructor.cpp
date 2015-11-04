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

#include <QUrl>

#include "qcommonsequencetypes_p.h"
#include "qnodebuilder_p.h"
#include "qqnamevalue_p.h"

#include "qattributeconstructor_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

AttributeConstructor::AttributeConstructor(const Expression::Ptr &op1,
                                           const Expression::Ptr &op2) : PairContainer(op1, op2)
{
}

QString AttributeConstructor::processValue(const QXmlName name,
                                           const Item &value)
{
    if(!value)
        return QString();
    else if(name == QXmlName(StandardNamespaces::xml, StandardLocalNames::id))
        return value.stringValue().simplified();
    else
        return value.stringValue();
}

Item AttributeConstructor::evaluateSingleton(const DynamicContext::Ptr &context) const
{
    const Item nameItem(m_operand1->evaluateSingleton(context));
    const Item content(m_operand2->evaluateSingleton(context));

    const QXmlName name(nameItem.as<QNameValue>()->qName());
    const QString value(processValue(name, content));
    const NodeBuilder::Ptr nodeBuilder(context->nodeBuilder(QUrl()));

    nodeBuilder->attribute(name, QStringRef(&value));

    const QAbstractXmlNodeModel::Ptr nm(nodeBuilder->builtDocument());
    context->addNodeModel(nm);
    return nm->root(QXmlNodeModelIndex());
}

void AttributeConstructor::evaluateToSequenceReceiver(const DynamicContext::Ptr &context) const
{
    QAbstractXmlReceiver *const receiver = context->outputReceiver();
    const Item nameItem(m_operand1->evaluateSingleton(context));

    const Item content(m_operand2->evaluateSingleton(context));
    const QXmlName name(nameItem.as<QNameValue>()->qName());
    const QString value(processValue(name, content));

    receiver->attribute(name, QStringRef(&value));
}

SequenceType::Ptr AttributeConstructor::staticType() const
{
    return CommonSequenceTypes::ExactlyOneAttribute;
}

SequenceType::List AttributeConstructor::expectedOperandTypes() const
{
    SequenceType::List result;
    result.append(CommonSequenceTypes::ExactlyOneQName);
    result.append(CommonSequenceTypes::ZeroOrMoreItems);
    return result;
}

Expression::Properties AttributeConstructor::properties() const
{
    return DisableElimination | IsNodeConstructor;
}

ExpressionVisitorResult::Ptr
AttributeConstructor::accept(const ExpressionVisitor::Ptr &visitor) const
{
    return visitor->visit(this);
}

Expression::ID AttributeConstructor::id() const
{
    return IDAttributeConstructor;
}

QT_END_NAMESPACE
