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

#include "qsqlindex.h"

#include "qsqlfield.h"
#include "qstringlist.h"

QT_BEGIN_NAMESPACE

/*!
    \class QSqlIndex
    \brief The QSqlIndex class provides functions to manipulate and
    describe database indexes.

    \ingroup database
    \inmodule QtSql

    An \e index refers to a single table or view in a database.
    Information about the fields that comprise the index can be used
    to generate SQL statements.
*/

/*!
    Constructs an empty index using the cursor name \a cursorname and
    index name \a name.
*/

QSqlIndex::QSqlIndex(const QString& cursorname, const QString& name)
    : cursor(cursorname), nm(name)
{
}

/*!
    Constructs a copy of \a other.
*/

QSqlIndex::QSqlIndex(const QSqlIndex& other)
    : QSqlRecord(other), cursor(other.cursor), nm(other.nm), sorts(other.sorts)
{
}

/*!
    Sets the index equal to \a other.
*/

QSqlIndex& QSqlIndex::operator=(const QSqlIndex& other)
{
    cursor = other.cursor;
    nm = other.nm;
    sorts = other.sorts;
    QSqlRecord::operator=(other);
    return *this;
}

/*!
    Destroys the object and frees any allocated resources.
*/

QSqlIndex::~QSqlIndex()
{

}

/*!
    Sets the name of the index to \a name.
*/

void QSqlIndex::setName(const QString& name)
{
    nm = name;
}

/*!
    \fn QString QSqlIndex::name() const

    Returns the name of the index.
*/

/*!
    Appends the field \a field to the list of indexed fields. The
    field is appended with an ascending sort order.
*/

void QSqlIndex::append(const QSqlField& field)
{
    append(field, false);
}

/*!
    \overload

    Appends the field \a field to the list of indexed fields. The
    field is appended with an ascending sort order, unless \a desc is
    true.
*/

void QSqlIndex::append(const QSqlField& field, bool desc)
{
    sorts.append(desc);
    QSqlRecord::append(field);
}


/*!
    Returns true if field \a i in the index is sorted in descending
    order; otherwise returns false.
*/

bool QSqlIndex::isDescending(int i) const
{
    if (i >= 0 && i < sorts.size())
        return sorts[i];
    return false;
}

/*!
    If \a desc is true, field \a i is sorted in descending order.
    Otherwise, field \a i is sorted in ascending order (the default).
    If the field does not exist, nothing happens.
*/

void QSqlIndex::setDescending(int i, bool desc)
{
    if (i >= 0 && i < sorts.size())
        sorts[i] = desc;
}


/*! \internal

  Creates a string representing the field number \a i using prefix \a
  prefix. If \a verbose is true, ASC or DESC is included in the field
  description if the field is sorted in ASCending or DESCending order.
*/

QString QSqlIndex::createField(int i, const QString& prefix, bool verbose) const
{
    QString f;
    if (!prefix.isEmpty())
        f += prefix + QLatin1Char('.');
    f += field(i).name();
    if (verbose)
        f += QLatin1Char(' ') + QString((isDescending(i)
                    ? QLatin1String("DESC") : QLatin1String("ASC")));
    return f;
}

/*!
    \fn QString QSqlIndex::cursorName() const

    Returns the name of the cursor which the index is associated with.
*/


/*!
    Sets the name of the cursor that the index is associated with to
    \a cursorName.
*/
void QSqlIndex::setCursorName(const QString& cursorName)
{
    cursor = cursorName;
}

QT_END_NAMESPACE
