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

#ifndef QInsertionIterator_P_H
#define QInsertionIterator_P_H

#include "qabstractxmlforwarditerator_p.h"
#include "qitem_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
     class InsertionIterator : public Item::Iterator
    {
    public:

        /**
         * Creates an InsertionIterator whose result is a merge of the
         * iterator @p insertIterator into the iterator @p target at position @p
         * position.
         *
         * @param target the iterator containing the items that the
         * item in @p insertIterator will be inserted into.
         * @param position the insertion position. Must be 1 or larger
         * @param insertIterator the iterator containing the items to insert
         * at position @p position
         */
        InsertionIterator(const Item::Iterator::Ptr &target,
                          const xsInteger position,
                          const Item::Iterator::Ptr &insertIterator);

        virtual Item next();
        virtual Item current() const;
        virtual xsInteger position() const;
        virtual xsInteger count();
        virtual Item::Iterator::Ptr copy() const;

    private:
        const Item::Iterator::Ptr m_target;
        const xsInteger m_insertPos;
        const Item::Iterator::Ptr m_inserts;
        Item m_current;
        xsInteger m_position;
        bool m_isInserting;
    };
}

QT_END_NAMESPACE

#endif
