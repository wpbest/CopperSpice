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

#ifndef Patternist_Boolean_P_H
#define Patternist_Boolean_P_H

#include "qitem_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{   
    class Boolean : public AtomicValue
    {
    public:
        typedef AtomicValue::Ptr Ptr;

        /**
         * @returns the boolean value this Boolean represents
         */
        static bool evaluateEBV(const Item::Iterator::Ptr &e,
                                const QExplicitlySharedDataPointer<DynamicContext> &);

        static bool evaluateEBV(const Item &first,
                                const Item::Iterator::Ptr &e,
                                const QExplicitlySharedDataPointer<DynamicContext> &);

        static bool evaluateEBV(const Item &item,
                                const QExplicitlySharedDataPointer<DynamicContext> &context);

        virtual QString stringValue() const;

        /**
         * @returns a Boolean object instantiating @p value. Use True() or False()
         * if you already know what value you need.
         */
        static Boolean::Ptr fromValue(const bool value);

        /**
         * Creates a boolean value from a lexical representation. "true" and "1"
         * becomes @c true, while "false" and "0" becomes @c false.
         */
        static AtomicValue::Ptr fromLexical(const QString &val);

        /**
         * Get the Effective %Boolean Value of this boolean value. For <tt>xs:boolean</tt>, this
         * is simply the value.
         */
        virtual bool evaluateEBV(const QExplicitlySharedDataPointer<DynamicContext> &) const;

        virtual ItemType::Ptr type() const;

        inline bool value() const
        {
            return m_value;
        }

    protected:
        friend class CommonValues;
        Boolean(const bool value);

    private:
        const bool m_value;
    };
}

QT_END_NAMESPACE

#endif
