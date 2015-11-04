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

#ifndef QDECLARATIVEBIND_H
#define QDECLARATIVEBIND_H

#include <qdeclarative.h>

#include <QtCore/qobject.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QDeclarativeBindPrivate;
class QDeclarativeBind : public QObject, public QDeclarativeParserStatus
{
    CS_OBJECT(QDeclarativeBind)
    Q_DECLARE_PRIVATE(QDeclarativeBind)
    CS_INTERFACES(QDeclarativeParserStatus)
    CS_PROPERTY_READ(*target, object)
    CS_PROPERTY_WRITE(*target, setObject)
    CS_PROPERTY_READ(property, property)
    CS_PROPERTY_WRITE(property, setProperty)
    CS_PROPERTY_READ(value, value)
    CS_PROPERTY_WRITE(value, setValue)
    CS_PROPERTY_READ(when, when)
    CS_PROPERTY_WRITE(when, setWhen)

public:
    QDeclarativeBind(QObject *parent=0);
    ~QDeclarativeBind();

    bool when() const;
    void setWhen(bool);

    QObject *object();
    void setObject(QObject *);

    QString property() const;
    void setProperty(const QString &);

    QVariant value() const;
    void setValue(const QVariant &);

protected:
    virtual void classBegin();
    virtual void componentComplete();

private:
    void eval();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeBind)

QT_END_HEADER

#endif
