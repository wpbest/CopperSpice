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

#ifndef QMULTITOUCH_MAC_P_H
#define QMULTITOUCH_MAC_P_H

#ifdef QT_MAC_USE_COCOA
#import <Cocoa/Cocoa.h>
#endif

#include <qevent.h>
#include <qhash.h>
#include <QtCore>

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6

QT_BEGIN_NAMESPACE

#ifdef QT_MAC_USE_COCOA

class QCocoaTouch
{
    public:
        static QList<QTouchEvent::TouchPoint> getCurrentTouchPointList(NSEvent *event, bool acceptSingleTouch);
        static void setMouseInDraggingState(bool inDraggingState);

    private:
        static QHash<qint64, QCocoaTouch*> _currentTouches;
        static QPointF _screenReferencePos;
        static QPointF _trackpadReferencePos;
        static int _idAssignmentCount;
        static int _touchCount;
        static bool _updateInternalStateOnly;

        QTouchEvent::TouchPoint _touchPoint;
        qint64 _identity;

        QCocoaTouch(NSTouch *nstouch);
        ~QCocoaTouch();

        void updateTouchData(NSTouch *nstouch, NSTouchPhase phase);
        static QCocoaTouch *findQCocoaTouch(NSTouch *nstouch);
        static Qt::TouchPointState toTouchPointState(NSTouchPhase nsState);
};

#endif // QT_MAC_USE_COCOA

QT_END_NAMESPACE

#endif // MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6

#endif // QMULTITOUCH_MAC_P_H

