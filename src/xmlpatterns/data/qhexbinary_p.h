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

#ifndef QHexBinary_P_H
#define QHexBinary_P_H

#include "qbase64binary_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{   
    class HexBinary : public Base64Binary
    {
    public:
        friend class CommonValues;

        typedef AtomicValue::Ptr Ptr;

        virtual QString stringValue() const;
        virtual ItemType::Ptr type() const;

        /**
         * Creates a @c xs:hexBinary from the lexical representation @p value.
         */
        static AtomicValue::Ptr fromLexical(const NamePool::Ptr &np, const QString &value);

        /**
         * Creates an instance representing @p value.
         */
        static HexBinary::Ptr fromValue(const QByteArray &data);

    protected:
        HexBinary(const QByteArray &val);

    private:
 
        static inline qint8 fromHex(const QChar &c);
    };
}

QT_END_NAMESPACE

#endif
