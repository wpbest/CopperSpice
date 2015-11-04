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

#ifndef QNETWORKCOOKIE_H
#define QNETWORKCOOKIE_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QList>
#include <QtCore/QMetaType>
#include <QtCore/QObject>

QT_BEGIN_NAMESPACE

class QByteArray;
class QDateTime;
class QString;
class QUrl;
class QNetworkCookiePrivate;

class Q_NETWORK_EXPORT QNetworkCookie
{
public:
    enum RawForm {
        NameAndValueOnly,
        Full
    };

    QNetworkCookie(const QByteArray &name = QByteArray(), const QByteArray &value = QByteArray());
    QNetworkCookie(const QNetworkCookie &other);
    ~QNetworkCookie();
    QNetworkCookie &operator=(const QNetworkCookie &other);
    bool operator==(const QNetworkCookie &other) const;
    inline bool operator!=(const QNetworkCookie &other) const
    { return !(*this == other); }

    bool isSecure() const;
    void setSecure(bool enable);
    bool isHttpOnly() const;
    void setHttpOnly(bool enable);

    bool isSessionCookie() const;
    QDateTime expirationDate() const;
    void setExpirationDate(const QDateTime &date);

    QString domain() const;
    void setDomain(const QString &domain);

    QString path() const;
    void setPath(const QString &path);

    QByteArray name() const;
    void setName(const QByteArray &cookieName);

    QByteArray value() const;
    void setValue(const QByteArray &value);

    QByteArray toRawForm(RawForm form = Full) const;

    static QList<QNetworkCookie> parseCookies(const QByteArray &cookieString);

private:
    QSharedDataPointer<QNetworkCookiePrivate> d;
    friend class QNetworkCookiePrivate;
};
Q_DECLARE_TYPEINFO(QNetworkCookie, Q_MOVABLE_TYPE);

#ifndef QT_NO_DEBUG_STREAM
class QDebug;
Q_NETWORK_EXPORT QDebug operator<<(QDebug, const QNetworkCookie &);
#endif

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QNetworkCookie)
Q_DECLARE_METATYPE(QList<QNetworkCookie>)

#endif
