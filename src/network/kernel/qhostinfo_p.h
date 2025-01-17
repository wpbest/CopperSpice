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

#ifndef QHOSTINFO_P_H
#define QHOSTINFO_P_H

#include "QtCore/qcoreapplication.h"
#include "qcoreapplication_p.h"
#include "QtNetwork/qhostinfo.h"
#include "QtCore/qmutex.h"
#include "QtCore/qwaitcondition.h"
#include "QtCore/qobject.h"
#include "QtCore/qpointer.h"
#include "QtCore/qthread.h"
#include "QtCore/qthreadpool.h"
#include "QtCore/qmutex.h"
#include "QtCore/qrunnable.h"
#include "QtCore/qlist.h"
#include "QtCore/qqueue.h"
#include <QElapsedTimer>
#include <QCache>
#include <QNetworkSession>
#include <QSharedPointer>

QT_BEGIN_NAMESPACE

class QHostInfoResult : public QObject
{
    CS_OBJECT(QHostInfoResult)

public :
    NET_CS_SLOT_1(Public, void emitResultsReady(const QHostInfo & info){emit resultsReady(info);})
    NET_CS_SLOT_2(emitResultsReady) 

    NET_CS_SIGNAL_1(Public, void resultsReady(const QHostInfo & info))
    NET_CS_SIGNAL_2(resultsReady,info) 
};

// needs to be QObject because fromName calls tr()
class QHostInfoAgent : public QObject
{
    CS_OBJECT(QHostInfoAgent)

public:
    static QHostInfo fromName(const QString &hostName);

#ifndef QT_NO_BEARERMANAGEMENT
    static QHostInfo fromName(const QString &hostName, QSharedPointer<QNetworkSession> networkSession);
#endif

};

class QHostInfoPrivate
{
public:
    inline QHostInfoPrivate()
        : err(QHostInfo::NoError),
          errorStr(QLatin1String(QT_TRANSLATE_NOOP("QHostInfo", "Unknown error"))),
          lookupId(0)
    {
    }
#ifndef QT_NO_BEARERMANAGEMENT
    //not a public API yet
    static QHostInfo fromName(const QString &hostName, QSharedPointer<QNetworkSession> networkSession);
#endif

    QHostInfo::HostInfoError err;
    QString errorStr;
    QList<QHostAddress> addrs;
    QString hostName;
    int lookupId;
};

// These functions are outside of the QHostInfo class and strictly internal.
// Do NOT use them outside of QAbstractSocket.
QHostInfo Q_NETWORK_EXPORT qt_qhostinfo_lookup(const QString &name, QObject *receiver, const char *member, bool *valid, int *id);
void qt_qhostinfo_clear_cache();
void qt_qhostinfo_enable_cache(bool e);

class QHostInfoCache
{
public:
    QHostInfoCache();
    const int max_age; // seconds

    QHostInfo get(const QString &name, bool *valid);
    void put(const QString &name, const QHostInfo &info);
    void clear();

    bool isEnabled();
    void setEnabled(bool e);
private:
    bool enabled;
    struct QHostInfoCacheElement {
        QHostInfo info;
        QElapsedTimer age;
    };
    QCache<QString,QHostInfoCacheElement> cache;
    QMutex mutex;
};

// the following classes are used for the (normal) case: We use multiple threads to lookup DNS

class QHostInfoRunnable : public QRunnable
{
public:
    QHostInfoRunnable (QString hn, int i);
    void run();

    QString toBeLookedUp;
    int id;
    QHostInfoResult resultEmitter;
};


class QAbstractHostInfoLookupManager : public QObject
{
    CS_OBJECT(QAbstractHostInfoLookupManager)

public:
    ~QAbstractHostInfoLookupManager() {}
    virtual void clear() = 0;

    QHostInfoCache cache;

protected:
     QAbstractHostInfoLookupManager() {}
     static QAbstractHostInfoLookupManager* globalInstance();

};


class QHostInfoLookupManager : public QAbstractHostInfoLookupManager
{
    CS_OBJECT(QHostInfoLookupManager)
public:
    QHostInfoLookupManager();
    ~QHostInfoLookupManager();

    void clear();
    void work();

    // called from QHostInfo
    void scheduleLookup(QHostInfoRunnable *r);
    void abortLookup(int id);

    // called from QHostInfoRunnable
    void lookupFinished(QHostInfoRunnable *r);
    bool wasAborted(int id);

    friend class QHostInfoRunnable;
protected:
    QList<QHostInfoRunnable*> currentLookups; // in progress
    QList<QHostInfoRunnable*> postponedLookups; // postponed because in progress for same host
    QQueue<QHostInfoRunnable*> scheduledLookups; // not yet started
    QList<QHostInfoRunnable*> finishedLookups; // recently finished
    QList<int> abortedLookups; // ids of aborted lookups

    QThreadPool threadPool;

    QMutex mutex;

    bool wasDeleted;

private :
    NET_CS_SLOT_1(Private, void waitForThreadPoolDone())
    NET_CS_SLOT_2(waitForThreadPoolDone) 
};


QT_END_NAMESPACE

#endif // QHOSTINFO_P_H
