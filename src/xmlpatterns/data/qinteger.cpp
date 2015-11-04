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

#include "qbuiltintypes_p.h"
#include "qitem_p.h"
#include "qvalidationerror_p.h"

#include "qinteger_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

Item Integer::fromValue(const xsInteger num)
{
    return toItem(Integer::Ptr(new Integer(num)));
}

AtomicValue::Ptr Integer::fromLexical(const QString &strNumeric)
{
    bool conversionOk = false;
    const xsInteger num = strNumeric.toLongLong(&conversionOk);

    if(conversionOk)
        return AtomicValue::Ptr(new Integer(num));
    else
        return ValidationError::createError();
}

Integer::Integer(const xsInteger num) : m_value(num)
{
}

bool Integer::evaluateEBV(const QExplicitlySharedDataPointer<DynamicContext> &) const
{
    return m_value != 0;
}

QString Integer::stringValue() const
{
    return QString::number(m_value);
}

ItemType::Ptr Integer::type() const
{
    return BuiltinTypes::xsInteger;
}

xsDouble Integer::toDouble() const
{
    return static_cast<xsDouble>(m_value);
}

xsInteger Integer::toInteger() const
{
    return m_value;
}

xsFloat Integer::toFloat() const
{
    return static_cast<xsFloat>(m_value);
}

xsDecimal Integer::toDecimal() const
{
    return static_cast<xsDecimal>(m_value);
}

Numeric::Ptr Integer::round() const
{
    /* xs:integerS never has a mantissa. */
    return Numeric::Ptr(const_cast<Integer *>(this));
}

Numeric::Ptr Integer::roundHalfToEven(const xsInteger /*scale*/) const
{
    return Numeric::Ptr(const_cast<Integer *>(this));
}

Numeric::Ptr Integer::floor() const
{
    return Numeric::Ptr(const_cast<Integer *>(this));
}

Numeric::Ptr Integer::ceiling() const
{
    return Numeric::Ptr(const_cast<Integer *>(this));
}

Numeric::Ptr Integer::abs() const
{
    /* No reason to allocate an Integer if we're already absolute. */
    if(m_value < 0)
        return Numeric::Ptr(new Integer(qAbs(m_value)));
    else
        return Numeric::Ptr(const_cast<Integer *>(this));
}

bool Integer::isNaN() const
{
    return false;
}

bool Integer::isInf() const
{
    return false;
}

Item Integer::toNegated() const
{
    return fromValue(-m_value);
}

bool Integer::isSigned() const
{
    return true;
}

qulonglong Integer::toUnsignedInteger() const
{
    Q_ASSERT_X(false, Q_FUNC_INFO,
               "It makes no sense to call this function, see Numeric::toUnsignedInteger().");
    return 0;
}

QT_END_NAMESPACE
