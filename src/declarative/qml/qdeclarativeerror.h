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

#ifndef QDECLARATIVEERROR_H
#define QDECLARATIVEERROR_H

#include <QtCore/qurl.h>
#include <QtCore/qstring.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QDebug;
class QDeclarativeErrorPrivate;

class Q_DECLARATIVE_EXPORT QDeclarativeError
{
public:
    QDeclarativeError();
    QDeclarativeError(const QDeclarativeError &);
    QDeclarativeError &operator=(const QDeclarativeError &);
    ~QDeclarativeError();

    bool isValid() const;

    QUrl url() const;
    void setUrl(const QUrl &);
    QString description() const;
    void setDescription(const QString &);
    int line() const;
    void setLine(int);
    int column() const;
    void setColumn(int);

    QString toString() const;

private:
    QDeclarativeErrorPrivate *d;
};

QDebug Q_DECLARATIVE_EXPORT operator<<(QDebug debug, const QDeclarativeError &error);

QT_END_NAMESPACE

QT_END_HEADER

#endif // QDECLARATIVEERROR_H
