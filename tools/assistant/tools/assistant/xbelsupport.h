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

#ifndef XBELSUPPORT_H
#define XBELSUPPORT_H

#include <QtXml/QXmlStreamReader>
#include <QtCore/QPersistentModelIndex>

QT_FORWARD_DECLARE_CLASS(QIODevice)
QT_FORWARD_DECLARE_CLASS(QModelIndex)

QT_BEGIN_NAMESPACE

class BookmarkModel;

class XbelWriter : public QXmlStreamWriter
{
public:
    XbelWriter(BookmarkModel *model);
    void writeToFile(QIODevice *device);

private:
    void writeData(const QModelIndex &index);

private:
    BookmarkModel *bookmarkModel;
};

class XbelReader : public QXmlStreamReader
{
public:
    XbelReader(BookmarkModel *model);
    bool readFromFile(QIODevice *device);

private:
    void readXBEL();
    void readFolder();
    void readBookmark();
    void readUnknownElement();

private:
    BookmarkModel *bookmarkModel;
    QList<QPersistentModelIndex> parents;
};

QT_END_NAMESPACE

#endif  // XBELSUPPORT_H
