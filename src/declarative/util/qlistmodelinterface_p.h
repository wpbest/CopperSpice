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

#ifndef QLISTMODELINTERFACE_H
#define QLISTMODELINTERFACE_H

#include <QtCore/QHash>
#include <QtCore/QVariant>

#include <private/qdeclarativeglobal_p.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class Q_DECLARATIVE_PRIVATE_EXPORT QListModelInterface : public QObject
{
    CS_OBJECT(QListModelInterface)

 public:
    QListModelInterface(QObject *parent = 0) : QObject(parent) {}
    virtual ~QListModelInterface() {}

    virtual int count() const = 0;
    virtual QVariant data(int index, int role) const = 0;

    virtual QList<int> roles() const = 0;
    virtual QString toString(int role) const = 0;
 
    CS_SIGNAL_1(Public, void itemsInserted(int index,int count))
    CS_SIGNAL_2(itemsInserted,index,count) 

    CS_SIGNAL_1(Public, void itemsRemoved(int index,int count))
    CS_SIGNAL_2(itemsRemoved,index,count) 

    CS_SIGNAL_1(Public, void itemsMoved(int from,int to,int count))
    CS_SIGNAL_2(itemsMoved,from,to,count) 

    CS_SIGNAL_1(Public, void itemsChanged(int index,int count,const QList <int> & roles))
    CS_SIGNAL_2(itemsChanged,index,count,roles) 

 protected:
    QListModelInterface(QObject *parent) 
        : QObject(parent) {}
};


QT_END_NAMESPACE

QT_END_HEADER
#endif //QTREEMODELINTERFACE_H
