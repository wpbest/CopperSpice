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

#ifndef Patternist_CacheCells_H
#define Patternist_CacheCells_H

#include <QList>
#include <QVector>
#include "qitem_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    class ItemCacheCell
    {
    public:
        typedef QList<ItemCacheCell> List;
        typedef QVector<ItemCacheCell> Vector;
        enum CacheState
        {
            Full,
            Empty
        };

        inline ItemCacheCell() : cacheState(Empty)
        {
        }

        Item        cachedItem;
        CacheState  cacheState;
    };

    class ItemSequenceCacheCell
    {
    public:
        typedef QList<ItemSequenceCacheCell> List;
        typedef QVector<ItemSequenceCacheCell> Vector;

        enum CacheState
        {
            Full,
            Empty,
            PartiallyPopulated
        };

        inline ItemSequenceCacheCell() : cacheState(Empty)
                                       , inUse(false)
        {
        }

        Item::List          cachedItems;
        Item::Iterator::Ptr sourceIterator;
        CacheState          cacheState;
 
        bool                inUse;
    };
}

Q_DECLARE_TYPEINFO(QPatternist::ItemCacheCell, Q_MOVABLE_TYPE);
Q_DECLARE_TYPEINFO(QPatternist::ItemSequenceCacheCell, Q_MOVABLE_TYPE);

QT_END_NAMESPACE


#endif
