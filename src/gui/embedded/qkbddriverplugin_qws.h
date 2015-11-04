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

#ifndef QKBDDRIVERPLUGIN_QWS_H
#define QKBDDRIVERPLUGIN_QWS_H

#include <QtCore/qplugin.h>
#include <QtCore/qfactoryinterface.h>

QT_BEGIN_NAMESPACE

class QWSKeyboardHandler;

struct Q_GUI_EXPORT QWSKeyboardHandlerFactoryInterface : public QFactoryInterface
{
    virtual QWSKeyboardHandler* create(const QString &name, const QString &device) = 0;
};

#define QWSKeyboardHandlerFactoryInterface_iid "com.copperspice.QWSKeyboardHandlerFactoryInterface"
CS_DECLARE_INTERFACE(QWSKeyboardHandlerFactoryInterface, QWSKeyboardHandlerFactoryInterface_iid)

class Q_GUI_EXPORT QKbdDriverPlugin : public QObject, public QWSKeyboardHandlerFactoryInterface
{
    CS_OBJECT_MULTIPLE(QKbdDriverPlugin, QObject)
    CS_INTERFACES(QWSKeyboardHandlerFactoryInterface, QFactoryInterface)

   public:
       explicit QKbdDriverPlugin(QObject *parent = 0);
       ~QKbdDriverPlugin();

       virtual QStringList keys() const = 0;
       virtual QWSKeyboardHandler* create(const QString& driver, const QString &device) = 0;
};

QT_END_NAMESPACE

#endif // QKBDDRIVERPLUGIN_QWS_H
