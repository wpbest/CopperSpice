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

#ifndef QFSFILEENGINE_ITERATOR_P_H
#define QFSFILEENGINE_ITERATOR_P_H

#include "qabstractfileengine.h"
#include "qfilesystemiterator_p.h"
#include "qdir.h"

#ifndef QT_NO_FSFILEENGINE
#ifndef QT_NO_FILESYSTEMITERATOR

QT_BEGIN_NAMESPACE

class QFSFileEngineIteratorPrivate;
class QFSFileEngineIteratorPlatformSpecificData;

class QFSFileEngineIterator : public QAbstractFileEngineIterator
{
   public:
       QFSFileEngineIterator(QDir::Filters filters, const QStringList &filterNames);
       ~QFSFileEngineIterator();
   
       QString next();
       bool hasNext() const;
   
       QString currentFileName() const;
       QFileInfo currentFileInfo() const;
   
   private:
       void advance() const;
       mutable QScopedPointer<QFileSystemIterator> nativeIterator;
       mutable QFileInfo currentInfo;
       mutable QFileInfo nextInfo;
       mutable bool done;
};

QT_END_NAMESPACE

#endif // QT_NO_FILESYSTEMITERATOR
#endif // QT_NO_FSFILEENGINE

#endif // QFSFILEENGINE_ITERATOR_P_H
