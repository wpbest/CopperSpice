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

#ifndef QDECLARATIVELISTMODEL_H
#define QDECLARATIVELISTMODEL_H

#include <qdeclarative.h>
#include <private/qdeclarativecustomparser_p.h>

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <private/qlistmodelinterface_p.h>
#include <QtScript/qscriptvalue.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class FlatListModel;
class NestedListModel;
class QDeclarativeListModelWorkerAgent;
struct ModelNode;
class FlatListScriptClass;

class Q_DECLARATIVE_PRIVATE_EXPORT QDeclarativeListModel : public QListModelInterface
{
    CS_OBJECT(QDeclarativeListModel)
    CS_PROPERTY_READ(count, count)
    CS_PROPERTY_NOTIFY(count, countChanged)

public:
    QDeclarativeListModel(QObject *parent=0);
    ~QDeclarativeListModel();

    virtual QList<int> roles() const;
    virtual QString toString(int role) const;
    virtual int count() const;
    virtual QVariant data(int index, int role) const;

    Q_INVOKABLE void clear();
    DECL_CS_INVOKABLE_METHOD_1(Public, )
    DECL_CS_INVOKABLE_METHOD_2()

    Q_INVOKABLE void remove(int index);
    DECL_CS_INVOKABLE_METHOD_1(Public, )
    DECL_CS_INVOKABLE_METHOD_2()

    Q_INVOKABLE void append(const QScriptValue&);
    DECL_CS_INVOKABLE_METHOD_1(Public, )
    DECL_CS_INVOKABLE_METHOD_2()

    Q_INVOKABLE void insert(int index, const QScriptValue&);
    DECL_CS_INVOKABLE_METHOD_1(Public, )
    DECL_CS_INVOKABLE_METHOD_2()

    Q_INVOKABLE QScriptValue get(int index) const;
    DECL_CS_INVOKABLE_METHOD_1(Public, )
    DECL_CS_INVOKABLE_METHOD_2()

    Q_INVOKABLE void set(int index, const QScriptValue&);
    DECL_CS_INVOKABLE_METHOD_1(Public, )
    DECL_CS_INVOKABLE_METHOD_2()

    Q_INVOKABLE void setProperty(int index, const QString& property, const QVariant& value);
    DECL_CS_INVOKABLE_METHOD_1(Public, )
    DECL_CS_INVOKABLE_METHOD_2()

    Q_INVOKABLE void move(int from, int to, int count);
    DECL_CS_INVOKABLE_METHOD_1(Public, )
    DECL_CS_INVOKABLE_METHOD_2()

    Q_INVOKABLE void sync();
    DECL_CS_INVOKABLE_METHOD_1(Public, )
    DECL_CS_INVOKABLE_METHOD_2()

    QDeclarativeListModelWorkerAgent *agent();

    CS_SIGNAL_1(Public, void countChanged())
    CS_SIGNAL_2(countChanged) 

private:
    friend class QDeclarativeListModelParser;
    friend class QDeclarativeListModelWorkerAgent;
    friend class FlatListModel;
    friend class FlatListScriptClass;
    friend struct ModelNode;

    // Constructs a flat list model for a worker agent
    QDeclarativeListModel(const QDeclarativeListModel *orig, QDeclarativeListModelWorkerAgent *parent);

    void set(int index, const QScriptValue&, QList<int> *roles);
    void setProperty(int index, const QString& property, const QVariant& value, QList<int> *roles);

    bool flatten();
    bool inWorkerThread() const;

    inline bool canMove(int from, int to, int n) const { return !(from+n > count() || to+n > count() || from < 0 || to < 0 || n < 0); }

    QDeclarativeListModelWorkerAgent *m_agent;
    NestedListModel *m_nested;
    FlatListModel *m_flat;
};

// ### FIXME
class QDeclarativeListElement : public QObject
{
CS_OBJECT(QDeclarativeListElement)
};

class QDeclarativeListModelParser : public QDeclarativeCustomParser
{
public:
    QByteArray compile(const QList<QDeclarativeCustomParserProperty> &);
    void setCustomData(QObject *, const QByteArray &);

private:
    struct ListInstruction
    {
        enum { Push, Pop, Value, Set } type;
        int dataIdx;
    };
    struct ListModelData
    {
        int dataOffset;
        int instrCount;
        ListInstruction *instructions() const;
    };
    bool compileProperty(const QDeclarativeCustomParserProperty &prop, QList<ListInstruction> &instr, QByteArray &data);

    bool definesEmptyList(const QString &);

    QByteArray listElementTypeName;
};


QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeListModel)
QML_DECLARE_TYPE(QDeclarativeListElement)

QT_END_HEADER

#endif // QDECLARATIVELISTMODEL_H
