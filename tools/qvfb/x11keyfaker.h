/***********************************************************************
*
* Copyright (c) 2012-2013 Barbara Geller
* Copyright (c) 2012-2013 Ansel Sermersheim
* Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef X11KEYFAKER_H
#define X11KEYFAKER_H

#include <QObject>
#include <QX11Info>
#include <qnamespace.h>

QT_BEGIN_NAMESPACE

class X11KeyFaker : public QObject
{
    Q_OBJECT
public:
    X11KeyFaker(const QString& displayName, QObject *parent = 0);
    ~X11KeyFaker();

    bool isConnected() const { return dpy != 0; }

    void sendKeyEvent(int qtCode, bool isPress);

private slots:
    void connect();
    void readyRead();

signals:
    void connected();
    void couldNotConnect();

private:
    QString displayName;
    Display *dpy;
    int retryCount;
    int shiftKeycode;
    int modeSwitchKeycode;
    int modifiers;
};

QT_END_NAMESPACE

#endif
