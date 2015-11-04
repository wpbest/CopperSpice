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

#ifndef QTHREADPOOL_H
#define QTHREADPOOL_H

#include <QtCore/qglobal.h>
#include <QtCore/qthread.h>
#include <QtCore/qrunnable.h>
#include <QScopedPointer>

QT_BEGIN_NAMESPACE

class QThreadPoolPrivate;

class Q_CORE_EXPORT QThreadPool : public QObject
{
    CS_OBJECT(QThreadPool)

    Q_DECLARE_PRIVATE(QThreadPool)

    CORE_CS_PROPERTY_READ(expiryTimeout, expiryTimeout)
    CORE_CS_PROPERTY_WRITE(expiryTimeout, setExpiryTimeout)
    CORE_CS_PROPERTY_READ(maxThreadCount, maxThreadCount)
    CORE_CS_PROPERTY_WRITE(maxThreadCount, setMaxThreadCount)
    CORE_CS_PROPERTY_READ(activeThreadCount, activeThreadCount)

    friend class QFutureInterfaceBase;

public:
    QThreadPool(QObject *parent = 0);
    ~QThreadPool();

    static QThreadPool *globalInstance();

    void start(QRunnable *runnable, int priority = 0);
    bool tryStart(QRunnable *runnable);

    int expiryTimeout() const;
    void setExpiryTimeout(int expiryTimeout);

    int maxThreadCount() const;
    void setMaxThreadCount(int maxThreadCount);

    int activeThreadCount() const;

    void reserveThread();
    void releaseThread();

    void waitForDone();
    bool waitForDone(int msecs);

protected:
	 QScopedPointer<QThreadPoolPrivate> d_ptr;

};

QT_END_NAMESPACE

#endif
