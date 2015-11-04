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

#ifndef Patternist_AbstractFloatMathematician_H
#define Patternist_AbstractFloatMathematician_H

#include "qabstractfloat_p.h"
#include "qatomicmathematician_p.h"
#include "qinteger_p.h"
#include "qschemanumeric_p.h"
#include "qpatternistlocale_p.h"
#include "qsourcelocationreflection_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
 
    template <const bool isDouble>
    class AbstractFloatMathematician : public AtomicMathematician
                                     , public DelegatingSourceLocationReflection
    {
    public:
        inline AbstractFloatMathematician(const SourceLocationReflection *const r) : DelegatingSourceLocationReflection(r)
        {
        }

        virtual Item calculate(const Item &o1,
                                    const Operator op,
                                    const Item &o2,
                                    const QExplicitlySharedDataPointer<DynamicContext> &context) const;
    };

#include "qabstractfloatmathematician.cpp"

    /**
     * An instantiation of AbstractFloatMathematician that handles @c xs:double.
     */
    typedef AbstractFloatMathematician<true> DoubleMathematician;

    /**
     * An instantiation of AbstractFloatMathematician that handles @c xs:float.
     */
    typedef AbstractFloatMathematician<false> FloatMathematician;
}

QT_END_NAMESPACE

#endif
