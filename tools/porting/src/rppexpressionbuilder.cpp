/***********************************************************************
*
* Copyright (c) 2012-2013 Barbara Geller
* Copyright (c) 2012-2013 Ansel Sermersheim
* Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "rppexpressionbuilder.h"
#include "tokens.h"
#include "rpp.h"

QT_BEGIN_NAMESPACE

using namespace TokenEngine;
namespace Rpp {

ExpressionBuilder::ExpressionBuilder(const TokenList &tokenList, const QVector<Type> &typeList, TypedPool<Item> *memoryPool)
:i(0)
,m_tokenList(tokenList)
,m_typeList(typeList)
,m_memoryPool(memoryPool)
{}

Rpp::Expression *ExpressionBuilder::parse()
{
    if(unary_expression_lookup())
        return conditional_expression();
    else
        return createIntLiteral(0);
}
Type ExpressionBuilder::next()
{
    if(!hasNext())
        return Token_eof;
    return typeAt(i++);
}

inline bool ExpressionBuilder::test(int token)
{
    if (i < m_tokenList.count() && typeAt(i) == token) {
        ++i;
        return true;
    }
    return false;
}

inline bool ExpressionBuilder::moreTokens(int delta)
{
    return (i + delta < m_tokenList.count());
}

inline Type ExpressionBuilder::lookup(int k)
{
    const int l = i - 1 + k;
    return l < m_tokenList.count() ? typeAt(l) : Token_eof;
}

Expression *ExpressionBuilder::conditional_expression()
{
    Expression *value = logical_OR_expression();
    if (test('?')) {
        Expression *leftExpression = conditional_expression();
        Expression *rightExpression;
        if(test(':'))
            rightExpression = conditional_expression();
        else
            rightExpression = createIntLiteral(0);
        return createConditionalExpression(value, leftExpression, rightExpression);
    }
    return value;
}

Expression *ExpressionBuilder::logical_OR_expression()
{
    Expression *value = logical_AND_expression();
    if (test(Token_or))
        return createBinaryExpression(Expression::OrOp, value, logical_OR_expression());
    return value;
}

Expression *ExpressionBuilder::logical_AND_expression()
{
    Expression *value = inclusive_OR_expression();
    if (test(Token_and))
        return createBinaryExpression(Expression::AndOp, value, logical_AND_expression());
    return value;
}

Expression *ExpressionBuilder::inclusive_OR_expression()
{
    Expression *value = exclusive_OR_expression();
    if (test('|'))
        return createBinaryExpression('|', value, inclusive_OR_expression());
    return value;
}

Expression *ExpressionBuilder::exclusive_OR_expression()
{
    Expression *value = AND_expression();
    if (test('^'))
        return createBinaryExpression('^', value, exclusive_OR_expression());
    return value;
}

Expression *ExpressionBuilder::AND_expression()
{
    Expression *value = equality_expression();
    if (test('&'))
        return createBinaryExpression('&', value, AND_expression());
    return value;
}

Expression *ExpressionBuilder::equality_expression()
{
    Expression *value = relational_expression();
    switch (next()) {
    case Token_eq:
        return createBinaryExpression(Expression::EqOp, value, equality_expression());
    case Token_not_eq:
        return createBinaryExpression(Expression::NotEqOp, value, equality_expression());
    default:
        prev();
        return value;
    }
}

Expression *ExpressionBuilder::relational_expression()
{
    Expression *value = shift_expression();
    switch (next()) {
    case '<':
        return createBinaryExpression('<', value, relational_expression());
    case '>':
        return createBinaryExpression('>', value, relational_expression());
    case Token_leq:
        return createBinaryExpression(Expression::LtEqOp, value, relational_expression());
    case Token_geq:
        return createBinaryExpression(Expression::GtEqOp, value, relational_expression());
    default:
        prev();
        return value;
    }
}

Expression *ExpressionBuilder::shift_expression()
{
    Expression *value = additive_expression();
    switch (next()) {
    case Token_left_shift:
        return createBinaryExpression(Expression::LShiftOp, value, shift_expression());
    case Token_right_shift:
        return createBinaryExpression(Expression::RShiftOp, value, shift_expression());
    default:
        prev();
        return value;
    }
}

Expression *ExpressionBuilder::additive_expression()
{
    Expression *value = multiplicative_expression();
    switch (next()) {
    case '+':
        return createBinaryExpression('+', value, additive_expression());
    case '-':
        return createBinaryExpression('-', value, additive_expression());
    default:
        prev();
        return value;
    }
}

Expression *ExpressionBuilder::multiplicative_expression()
{
    Expression *value = unary_expression();
    switch (next()) {
    case '*':
        return createBinaryExpression('*', value, multiplicative_expression());
    case '%':
        return createBinaryExpression('%', value, multiplicative_expression());
    case '/':
        return createBinaryExpression('/', value, multiplicative_expression());
    default:
        prev();
        return value;
    };
}

Expression *ExpressionBuilder::unary_expression()
{
    switch (next()) {
    case '+':
        return createUnaryExpression('+', unary_expression());
    case '-':
        return createUnaryExpression('-',  unary_expression());
    case '!':
        return createUnaryExpression('!',  unary_expression());
    case '~':
        return createUnaryExpression('~', unary_expression());
    case Token_defined:
    {
        int identifierIndex  = 0;
        if (test(Token_identifier)) {
            identifierIndex = i - 1;
        } else if (test('(')) {
            if (test(Token_identifier))
                identifierIndex = i -1;
            test(')');
        }
        return createMacroReference(MacroReference::DefinedRef, createTokenList(identifierIndex));
    }
    default:
        prev();
        return primary_expression();
    }
}

bool ExpressionBuilder::unary_expression_lookup()
{
    Type t = lookup();
    return (primary_expression_lookup()
            || t == '+'
            || t == '-'
            || t == '!'
            || t == '~'
            || t == Token_defined);
}

Expression *ExpressionBuilder::primary_expression()
{
    Expression *value;
    if (test('(')) {
        if (moreTokens(1))
            value = conditional_expression();
        else
            value = createIntLiteral(0); // Syntax error.
        test(')');
    } else {
        next();
        bool ok;
        int val  = QString::fromLatin1(lexem()).toInt(&ok, 0);
        if(ok)
            value = createIntLiteral(val);
        else
            value = createMacroReference(MacroReference::ValueRef, createTokenList(i -1));
    }
    return value;
}

bool ExpressionBuilder::primary_expression_lookup()
{
    Type t = lookup();
    return (t == Token_identifier
            || t == Token_number_literal
/*            || t == PP_FLOATING_LITERAL*/
            || t == '(');
}
/*
    Creates a tokenList containing one token
*/
TokenList ExpressionBuilder::createTokenList(int tokenIndex) const
{
    return TokenList(m_tokenList.tokenContainer(tokenIndex),
        QVector<int>() << m_tokenList.containerIndex(tokenIndex));
}
/*
    Node cration helper functions
*/
UnaryExpression *ExpressionBuilder::createUnaryExpression(int op, Expression *expression)
{
    return new (m_memoryPool->allocate(sizeof(UnaryExpression))) UnaryExpression(op, expression);
}

BinaryExpression *ExpressionBuilder::createBinaryExpression(int op, Expression *leftExpresson, Expression *rightExpression)
{
    return new (m_memoryPool->allocate(sizeof(BinaryExpression))) BinaryExpression(op, leftExpresson, rightExpression);
}

ConditionalExpression *ExpressionBuilder::createConditionalExpression(Expression *condition, Expression *leftExpression, Expression *rightExpression)
{
    return new (m_memoryPool->allocate(sizeof(ConditionalExpression))) ConditionalExpression(condition, leftExpression, rightExpression);
}

MacroReference *ExpressionBuilder::createMacroReference(MacroReference::Type type, TokenEngine::TokenList token)
{
    return new (m_memoryPool->allocate(sizeof(MacroReference))) MacroReference(token, type);
}

IntLiteral *ExpressionBuilder::createIntLiteral(const int arg)
{
    return new (m_memoryPool->allocate(sizeof(IntLiteral))) IntLiteral(arg);
}

}

QT_END_NAMESPACE
