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

#ifndef QNETWORKACCESSFTPBACKEND_P_H
#define QNETWORKACCESSFTPBACKEND_P_H

#include "qnetworkaccessbackend_p.h"
#include "qnetworkaccesscache_p.h"
#include "qnetworkrequest.h"
#include "qnetworkreply.h"
#include "QtNetwork/qftp.h"

#include "QtCore/qpointer.h"

#ifndef QT_NO_FTP

QT_BEGIN_NAMESPACE

class QNetworkAccessFtpIODevice;
class QNetworkAccessCachedFtpConnection;

class QNetworkAccessFtpBackend: public QNetworkAccessBackend
{
    CS_OBJECT(QNetworkAccessFtpBackend)

public:
    enum State {
        Idle,
        //Connecting,
        LoggingIn,
        CheckingFeatures,
        Statting,
        Transferring,
        Disconnecting
    };

    QNetworkAccessFtpBackend();
    virtual ~QNetworkAccessFtpBackend();

    virtual void open();
    virtual void closeDownstreamChannel();

    virtual void downstreamReadyWrite();

    void disconnectFromFtp();

public :
    NET_CS_SLOT_1(Public, void ftpConnectionReady(QNetworkAccessCache::CacheableObject * object))
    NET_CS_SLOT_2(ftpConnectionReady) 
    NET_CS_SLOT_1(Public, void ftpDone())
    NET_CS_SLOT_2(ftpDone) 
    NET_CS_SLOT_1(Public, void ftpReadyRead())
    NET_CS_SLOT_2(ftpReadyRead) 
    NET_CS_SLOT_1(Public, void ftpRawCommandReply(int code,const QString & text))
    NET_CS_SLOT_2(ftpRawCommandReply) 

private:
    friend class QNetworkAccessFtpIODevice;
    QPointer<QNetworkAccessCachedFtpConnection> ftp;
    QIODevice *uploadDevice;
    qint64 totalBytes;
    int helpId, sizeId, mdtmId;
    bool supportsSize, supportsMdtm;
    State state;
};

class QNetworkAccessFtpBackendFactory: public QNetworkAccessBackendFactory
{
public:
    virtual QNetworkAccessBackend *create(QNetworkAccessManager::Operation op,
                                          const QNetworkRequest &request) const;
};

QT_END_NAMESPACE

#endif // QT_NO_FTP

#endif
