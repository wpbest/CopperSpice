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

#include "qfinalstate.h"

#ifndef QT_NO_STATEMACHINE

#include "qabstractstate_p.h"

QT_BEGIN_NAMESPACE

class QFinalStatePrivate : public QAbstractStatePrivate
{
    Q_DECLARE_PUBLIC(QFinalState)

public:
    QFinalStatePrivate();
};

QFinalStatePrivate::QFinalStatePrivate()
    : QAbstractStatePrivate(FinalState)
{
}

/*!
  Constructs a new QFinalState object with the given \a parent state.
*/
QFinalState::QFinalState(QState *parent)
    : QAbstractState(*new QFinalStatePrivate, parent)
{
}

/*!
  Destroys this final state.
*/
QFinalState::~QFinalState()
{
}

/*!
  \reimp
*/
void QFinalState::onEntry(QEvent *event)
{
    Q_UNUSED(event);
}

/*!
  \reimp
*/
void QFinalState::onExit(QEvent *event)
{
    Q_UNUSED(event);
}

/*!
  \reimp
*/
bool QFinalState::event(QEvent *e)
{
    return QAbstractState::event(e);
}

QT_END_NAMESPACE

#endif //QT_NO_STATEMACHINE
