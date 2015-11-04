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

#ifndef QMACGESTURERECOGNIZER_MAC_P_H
#define QMACGESTURERECOGNIZER_MAC_P_H

#include "qtimer.h"
#include "qpoint.h"
#include "qgesturerecognizer.h"

#ifndef QT_NO_GESTURES

QT_BEGIN_NAMESPACE

class QMacSwipeGestureRecognizer : public QGestureRecognizer
{
public:
    QMacSwipeGestureRecognizer();

    QGesture *create(QObject *target);
    QGestureRecognizer::Result recognize(QGesture *gesture, QObject *watched, QEvent *event);
    void reset(QGesture *gesture);
};

class QMacPinchGestureRecognizer : public QGestureRecognizer
{
public:
    QMacPinchGestureRecognizer();

    QGesture *create(QObject *target);
    QGestureRecognizer::Result recognize(QGesture *gesture, QObject *watched, QEvent *event);
    void reset(QGesture *gesture);
};

#if defined(QT_MAC_USE_COCOA)

class QMacPanGestureRecognizer : public QObject, public QGestureRecognizer
{
public:
    QMacPanGestureRecognizer();

    QGesture *create(QObject *target);
    QGestureRecognizer::Result recognize(QGesture *gesture, QObject *watched, QEvent *event);
    void reset(QGesture *gesture);
private:
    QPointF _startPos;
    QBasicTimer _panTimer;
    bool _panCanceled;
};

#endif

QT_END_NAMESPACE

#endif // QT_NO_GESTURES

#endif // QMACSWIPEGESTURERECOGNIZER_MAC_P_H
