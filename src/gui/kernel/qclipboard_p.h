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

#ifndef QCLIPBOARD_P_H
#define QCLIPBOARD_P_H

#include "QtGui/qmime.h"
#include "QtGui/qclipboard.h"

QT_BEGIN_NAMESPACE

class QClipboardPrivate;

class QMimeDataWrapper : public QMimeSource
{
public:
    QMimeDataWrapper() {}

    const char* format(int n) const;
    QByteArray encodedData(const char*) const;

    mutable QList<QByteArray> formats;
    const QMimeData *data;
};

class QMimeSourceWrapper : public QMimeData
{
public:
    QMimeSourceWrapper(QClipboardPrivate *priv, QClipboard::Mode m);
    ~QMimeSourceWrapper();

    bool hasFormat(const QString &mimetype) const;
    QStringList formats() const;

protected:
    QVariant retrieveData(const QString &mimetype, QVariant::Type) const;
private:
    QClipboardPrivate *d;
    QClipboard::Mode mode;
    QMimeSource *source;
};


class QClipboardPrivate
{

public:
    QClipboardPrivate() {
        for (int i = 0; i <= QClipboard::LastMode; ++i) {
            compat_data[i] = 0;
            wrapper[i] = new QMimeDataWrapper();
        }
    }

    virtual ~QClipboardPrivate() {
        for (int i = 0; i <= QClipboard::LastMode; ++i) {
            delete wrapper[i];
            delete compat_data[i];
        }
    }

    mutable QMimeDataWrapper *wrapper[QClipboard::LastMode + 1];
    mutable QMimeSource *compat_data[QClipboard::LastMode + 1];
};

inline QMimeSourceWrapper::QMimeSourceWrapper(QClipboardPrivate *priv, QClipboard::Mode m)
    : QMimeData()
{
    d = priv;
    mode = m;
    source = d->compat_data[mode];
}

inline QMimeSourceWrapper::~QMimeSourceWrapper()
{
    if (d->compat_data[mode] == source)
        d->compat_data[mode] = 0;
    delete source;
}

QT_END_NAMESPACE

#endif // QCLIPBOARD_P_H
