/*
    Copyright (C) 2007 Staikos Computing Services, Inc.  <info@staikos.net>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.

    This class provides all functionality needed for tracking global history.
*/

#ifndef QWEBHISTORYINTERFACE_H
#define QWEBHISTORYINTERFACE_H

#include <QtCore/qobject.h>

#include "qwebkitglobal.h"

class QWEBKIT_EXPORT QWebHistoryInterface : public QObject {
    CS_OBJECT(QWebHistoryInterface)
public:
    QWebHistoryInterface(QObject *parent = 0);
    ~QWebHistoryInterface();

    static void setDefaultInterface(QWebHistoryInterface *defaultInterface);
    static QWebHistoryInterface *defaultInterface();

    virtual bool historyContains(const QString &url) const = 0;
    virtual void addHistoryEntry(const QString &url) = 0;
};

#endif
