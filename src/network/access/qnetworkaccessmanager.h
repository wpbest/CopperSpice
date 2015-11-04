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

#ifndef QNETWORKACCESSMANAGER_H
#define QNETWORKACCESSMANAGER_H

#include <QtCore/QObject>
#include <QNetworkSession>
#include "qsslerror.h"
#include <QScopedPointer>

QT_BEGIN_NAMESPACE

class QIODevice;
class QAbstractNetworkCache;
class QAuthenticator;
class QByteArray;
class QNetworkCookieJar;
class QNetworkRequest;
class QNetworkReply;
class QNetworkProxy;
class QNetworkProxyFactory;
class QHttpMultiPart;
class QNetworkReplyImplPrivate;
class QNetworkAccessManagerPrivate;

template<typename T> class QList;
class QNetworkCookie;

#if !defined(QT_NO_BEARERMANAGEMENT) && !defined(QT_MOBILITY_BEARER)
class QNetworkConfiguration;
#endif

class Q_NETWORK_EXPORT QNetworkAccessManager: public QObject
{
    CS_OBJECT(QNetworkAccessManager)

#ifndef QT_NO_BEARERMANAGEMENT
    NET_CS_PROPERTY_READ(networkAccessible, networkAccessible)
    NET_CS_PROPERTY_WRITE(networkAccessible, setNetworkAccessible)
    NET_CS_PROPERTY_NOTIFY(networkAccessible, networkAccessibleChanged)
#endif

public:
    enum Operation {
        HeadOperation = 1,
        GetOperation,
        PutOperation,
        PostOperation,
        DeleteOperation,
        CustomOperation,

        UnknownOperation = 0
    };

#ifndef QT_NO_BEARERMANAGEMENT
    enum NetworkAccessibility {
        UnknownAccessibility = -1,
        NotAccessible = 0,
        Accessible = 1
    };
#endif

    explicit QNetworkAccessManager(QObject *parent = 0);
    ~QNetworkAccessManager();

#ifndef QT_NO_NETWORKPROXY
    QNetworkProxy proxy() const;
    void setProxy(const QNetworkProxy &proxy);
    QNetworkProxyFactory *proxyFactory() const;
    void setProxyFactory(QNetworkProxyFactory *factory);
#endif

    QAbstractNetworkCache *cache() const;
    void setCache(QAbstractNetworkCache *cache);

    QNetworkCookieJar *cookieJar() const;
    void setCookieJar(QNetworkCookieJar *cookieJar);

    QNetworkReply *head(const QNetworkRequest &request);
    QNetworkReply *get(const QNetworkRequest &request);
    QNetworkReply *post(const QNetworkRequest &request, QIODevice *data);
    QNetworkReply *post(const QNetworkRequest &request, const QByteArray &data);
    QNetworkReply *post(const QNetworkRequest &request, QHttpMultiPart *multiPart);
    QNetworkReply *put(const QNetworkRequest &request, QIODevice *data);
    QNetworkReply *put(const QNetworkRequest &request, const QByteArray &data);
    QNetworkReply *put(const QNetworkRequest &request, QHttpMultiPart *multiPart);
    QNetworkReply *deleteResource(const QNetworkRequest &request);
    QNetworkReply *sendCustomRequest(const QNetworkRequest &request, const QByteArray &verb, QIODevice *data = 0);

    NET_CS_SIGNAL_1(Public, void authenticationRequired(QNetworkReply * reply,QAuthenticator * authenticator))
    NET_CS_SIGNAL_2(authenticationRequired,reply,authenticator) 
    NET_CS_SIGNAL_1(Public, void finished(QNetworkReply * reply))
    NET_CS_SIGNAL_2(finished,reply) 

#if !defined(QT_NO_BEARERMANAGEMENT) && !defined(QT_MOBILITY_BEARER)
    void setConfiguration(const QNetworkConfiguration &config);
    QNetworkConfiguration configuration() const;
    QNetworkConfiguration activeConfiguration() const;
#endif

#ifndef QT_NO_BEARERMANAGEMENT
    void setNetworkAccessible(NetworkAccessibility accessible);
    NetworkAccessibility networkAccessible() const;
#endif

#ifndef QT_NO_NETWORKPROXY
    NET_CS_SIGNAL_1(Public, void proxyAuthenticationRequired(const QNetworkProxy & proxy,QAuthenticator * authenticator))
    NET_CS_SIGNAL_2(proxyAuthenticationRequired,proxy,authenticator) 
#endif 

#ifndef QT_NO_OPENSSL
    NET_CS_SIGNAL_1(Public, void sslErrors(QNetworkReply * reply,const QList <QSslError> & errors))
    NET_CS_SIGNAL_2(sslErrors,reply,errors) 
#endif

#if !defined(QT_NO_BEARERMANAGEMENT) && !defined(QT_MOBILITY_BEARER)
    NET_CS_SIGNAL_1(Public, void networkSessionConnected())
    NET_CS_SIGNAL_2(networkSessionConnected)
#endif

#ifndef QT_NO_BEARERMANAGEMENT
    NET_CS_SIGNAL_1(Public, void networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible))
    NET_CS_SIGNAL_2(networkAccessibleChanged,accessible) 
#endif

protected:
    virtual QNetworkReply *createRequest(Operation op, const QNetworkRequest &request,QIODevice *outgoingData = 0);
    QScopedPointer<QNetworkAccessManagerPrivate> d_ptr;

private:
    friend class QNetworkReplyImplPrivate;
    friend class QNetworkAccessHttpBackend;

    Q_DECLARE_PRIVATE(QNetworkAccessManager)

    NET_CS_SLOT_1(Private, void _q_replyFinished())
    NET_CS_SLOT_2(_q_replyFinished)

#ifndef QT_NO_OPENSSL
    NET_CS_SLOT_1(Private, void _q_replySslErrors(const QList <QSslError> &un_named_arg1))
    NET_CS_SLOT_2(_q_replySslErrors)
#endif

#if ! defined(QT_NO_BEARERMANAGEMENT) && !defined(QT_MOBILITY_BEARER)
    NET_CS_SLOT_1(Private, void _q_networkSessionClosed())
    NET_CS_SLOT_2(_q_networkSessionClosed)

    NET_CS_SLOT_1(Private, void _q_networkSessionStateChanged(QNetworkSession::State un_named_arg1))
    NET_CS_SLOT_2(_q_networkSessionStateChanged)
#endif
 

};

QT_END_NAMESPACE

#endif
