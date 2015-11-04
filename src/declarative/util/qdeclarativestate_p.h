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

#ifndef QDECLARATIVESTATE_H
#define QDECLARATIVESTATE_H

#include <qdeclarative.h>
#include <qdeclarativeproperty.h>
#include <QtCore/qobject.h>
#include <QtCore/qsharedpointer.h>
#include <private/qdeclarativeglobal_p.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QDeclarativeActionEvent;
class QDeclarativeAbstractBinding;
class QDeclarativeBinding;
class QDeclarativeExpression;

class Q_DECLARATIVE_PRIVATE_EXPORT QDeclarativeAction
{
public:
    QDeclarativeAction();
    QDeclarativeAction(QObject *, const QString &, const QVariant &);
    QDeclarativeAction(QObject *, const QString &,
                       QDeclarativeContext *, const QVariant &);

    bool restore:1;
    bool actionDone:1;
    bool reverseEvent:1;
    bool deletableToBinding:1;

    QDeclarativeProperty property;
    QVariant fromValue;
    QVariant toValue;

    QDeclarativeAbstractBinding *fromBinding;
    QWeakPointer<QDeclarativeAbstractBinding> toBinding;
    QDeclarativeActionEvent *event;

    //strictly for matching
    QObject *specifiedObject;
    QString specifiedProperty;

    void deleteFromBinding();
};

class QDeclarativeActionEvent
{
public:
    virtual ~QDeclarativeActionEvent();
    virtual QString typeName() const;

    enum Reason { ActualChange, FastForward };

    virtual void execute(Reason reason = ActualChange);
    virtual bool isReversable();
    virtual void reverse(Reason reason = ActualChange);
    virtual void saveOriginals() {}
    virtual bool needsCopy() { return false; }
    virtual void copyOriginals(QDeclarativeActionEvent *) {}

    virtual bool isRewindable() { return isReversable(); }
    virtual void rewind() {}
    virtual void saveCurrentValues() {}
    virtual void saveTargetValues() {}

    virtual bool changesBindings();
    virtual void clearBindings();
    virtual bool override(QDeclarativeActionEvent*other);
};

//### rename to QDeclarativeStateChange?
class QDeclarativeStateGroup;
class QDeclarativeState;
class QDeclarativeStateOperationPrivate;
class Q_DECLARATIVE_EXPORT QDeclarativeStateOperation : public QObject
{
    CS_OBJECT(QDeclarativeStateOperation)
public:
    QDeclarativeStateOperation(QObject *parent = 0)
        : QObject(parent) {}
    typedef QList<QDeclarativeAction> ActionList;

    virtual ActionList actions();

    QDeclarativeState *state() const;
    void setState(QDeclarativeState *state);

protected:
    QDeclarativeStateOperation(QObjectPrivate &dd, QObject *parent = 0);

private:
    Q_DECLARE_PRIVATE(QDeclarativeStateOperation)
    Q_DISABLE_COPY(QDeclarativeStateOperation)
};

typedef QDeclarativeStateOperation::ActionList QDeclarativeStateActions;

class QDeclarativeTransition;
class QDeclarativeStatePrivate;
class Q_DECLARATIVE_EXPORT QDeclarativeState : public QObject
{
    CS_OBJECT(QDeclarativeState)

    CS_PROPERTY_READ(name, name)
    CS_PROPERTY_WRITE(name, setName)
    CS_PROPERTY_READ(*when, when)
    CS_PROPERTY_WRITE(*when, setWhen)
    CS_PROPERTY_READ(extend, extends)
    CS_PROPERTY_WRITE(extend, setExtends)
    CS_PROPERTY_READ(changes, changes)
    CS_CLASSINFO("DefaultProperty", "changes")
    CS_CLASSINFO("DeferredPropertyNames", "changes")

public:
    QDeclarativeState(QObject *parent=0);
    virtual ~QDeclarativeState();

    QString name() const;
    void setName(const QString &);
    bool isNamed() const;

    /*'when' is a QDeclarativeBinding to limit state changes oscillation
     due to the unpredictable order of evaluation of bound expressions*/
    bool isWhenKnown() const;
    QDeclarativeBinding *when() const;
    void setWhen(QDeclarativeBinding *);

    QString extends() const;
    void setExtends(const QString &);

    QDeclarativeListProperty<QDeclarativeStateOperation> changes();
    int operationCount() const;
    QDeclarativeStateOperation *operationAt(int) const;

    QDeclarativeState &operator<<(QDeclarativeStateOperation *);

    void apply(QDeclarativeStateGroup *, QDeclarativeTransition *, QDeclarativeState *revert);
    void cancel();

    QDeclarativeStateGroup *stateGroup() const;
    void setStateGroup(QDeclarativeStateGroup *);

    bool containsPropertyInRevertList(QObject *target, const QString &name) const;
    bool changeValueInRevertList(QObject *target, const QString &name, const QVariant &revertValue);
    bool changeBindingInRevertList(QObject *target, const QString &name, QDeclarativeAbstractBinding *binding);
    bool removeEntryFromRevertList(QObject *target, const QString &name);
    void addEntryToRevertList(const QDeclarativeAction &action);
    void removeAllEntriesFromRevertList(QObject *target);
    void addEntriesToRevertList(const QList<QDeclarativeAction> &actions);
    QVariant valueInRevertList(QObject *target, const QString &name) const;
    QDeclarativeAbstractBinding *bindingInRevertList(QObject *target, const QString &name) const;

    bool isStateActive() const;

public:
    CS_SIGNAL_1(Public, void completed())
    CS_SIGNAL_2(completed) 

private:
    Q_DECLARE_PRIVATE(QDeclarativeState)
    Q_DISABLE_COPY(QDeclarativeState)
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeStateOperation)
QML_DECLARE_TYPE(QDeclarativeState)

QT_END_HEADER

#endif // QDECLARATIVESTATE_H
