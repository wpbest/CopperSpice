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

#ifndef QDECLARATIVEINFO_H
#define QDECLARATIVEINFO_H

#include <QtCore/qdebug.h>
#include <QtCore/qurl.h>
#include <QtDeclarative/qdeclarativeerror.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QDeclarativeInfoPrivate;
class Q_DECLARATIVE_EXPORT QDeclarativeInfo : public QDebug
{
public:
    QDeclarativeInfo(const QDeclarativeInfo &);
    ~QDeclarativeInfo();

    inline QDeclarativeInfo &operator<<(QChar t) { QDebug::operator<<(t); return *this; }
    inline QDeclarativeInfo &operator<<(QBool t) { QDebug::operator<<(t); return *this; }
    inline QDeclarativeInfo &operator<<(bool t) { QDebug::operator<<(t); return *this; }
    inline QDeclarativeInfo &operator<<(char t) { QDebug::operator<<(t); return *this; }
    inline QDeclarativeInfo &operator<<(signed short t) { QDebug::operator<<(t); return *this; }
    inline QDeclarativeInfo &operator<<(unsigned short t) { QDebug::operator<<(t); return *this; }
    inline QDeclarativeInfo &operator<<(signed int t) { QDebug::operator<<(t); return *this; }
    inline QDeclarativeInfo &operator<<(unsigned int t) { QDebug::operator<<(t); return *this; }
    inline QDeclarativeInfo &operator<<(signed long t) { QDebug::operator<<(t); return *this; }
    inline QDeclarativeInfo &operator<<(unsigned long t) { QDebug::operator<<(t); return *this; }
    inline QDeclarativeInfo &operator<<(qint64 t) { QDebug::operator<<(t); return *this; }
    inline QDeclarativeInfo &operator<<(quint64 t) { QDebug::operator<<(t); return *this; }
    inline QDeclarativeInfo &operator<<(float t) { QDebug::operator<<(t); return *this; }
    inline QDeclarativeInfo &operator<<(double t) { QDebug::operator<<(t); return *this; }
    inline QDeclarativeInfo &operator<<(const char* t) { QDebug::operator<<(t); return *this; }
    inline QDeclarativeInfo &operator<<(const QString & t) { QDebug::operator<<(t.toLocal8Bit().constData()); return *this; }
    inline QDeclarativeInfo &operator<<(const QStringRef & t) { return operator<<(t.toString()); }
    inline QDeclarativeInfo &operator<<(const QLatin1String &t) { QDebug::operator<<(t.latin1()); return *this; }
    inline QDeclarativeInfo &operator<<(const QByteArray & t) { QDebug::operator<<(t); return *this; }
    inline QDeclarativeInfo &operator<<(const void * t) { QDebug::operator<<(t); return *this; }
    inline QDeclarativeInfo &operator<<(QTextStreamFunction f) { QDebug::operator<<(f); return *this; }
    inline QDeclarativeInfo &operator<<(QTextStreamManipulator m) { QDebug::operator<<(m); return *this; }
#ifndef QT_NO_DEBUG_STREAM
    inline QDeclarativeInfo &operator<<(const QUrl &t) { static_cast<QDebug &>(*this) << t; return *this; }
#endif

private:
    friend Q_DECLARATIVE_EXPORT QDeclarativeInfo qmlInfo(const QObject *me);
    friend Q_DECLARATIVE_EXPORT QDeclarativeInfo qmlInfo(const QObject *me, const QDeclarativeError &error);
    friend Q_DECLARATIVE_EXPORT QDeclarativeInfo qmlInfo(const QObject *me, const QList<QDeclarativeError> &errors);

    QDeclarativeInfo(QDeclarativeInfoPrivate *);
    QDeclarativeInfoPrivate *d;
};

Q_DECLARATIVE_EXPORT QDeclarativeInfo qmlInfo(const QObject *me);
Q_DECLARATIVE_EXPORT QDeclarativeInfo qmlInfo(const QObject *me, const QDeclarativeError &error);
Q_DECLARATIVE_EXPORT QDeclarativeInfo qmlInfo(const QObject *me, const QList<QDeclarativeError> &errors);

QT_END_NAMESPACE

QT_END_HEADER

#endif // QDECLARATIVEINFO_H
