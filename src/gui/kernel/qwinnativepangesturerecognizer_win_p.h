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

#ifndef QWINNATIVEPANGESTURERECOGNIZER_WIN_P_H
#define QWINNATIVEPANGESTURERECOGNIZER_WIN_P_H

#include <QGestureRecognizer>

#ifndef QT_NO_GESTURES

QT_BEGIN_NAMESPACE

#if !defined(QT_NO_NATIVE_GESTURES)

class QWinNativePanGestureRecognizer : public QGestureRecognizer
{
public:
    QWinNativePanGestureRecognizer();

    QGesture *create(QObject *target);
    QGestureRecognizer::Result recognize(QGesture *state, QObject *watched, QEvent *event);
    void reset(QGesture *state);
};

#endif // QT_NO_NATIVE_GESTURES

QT_END_NAMESPACE

#endif // QT_NO_GESTURES

#endif // QWINNATIVEPANGESTURERECOGNIZER_WIN_P_H
