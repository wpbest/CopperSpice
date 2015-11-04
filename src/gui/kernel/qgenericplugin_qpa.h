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

#ifndef QGENERICPLUGIN_QPA_H
#define QGENERICPLUGIN_QPA_H

#include <QtCore/qplugin.h>
#include <QtCore/qfactoryinterface.h>

QT_BEGIN_NAMESPACE

struct Q_GUI_EXPORT QGenericPluginFactoryInterface : public QFactoryInterface
{
    virtual QObject* create(const QString &name, const QString &spec) = 0;
};

#define QGenericPluginFactoryInterface_iid "com.copperspice.QGenericPluginFactoryInterface"
CS_DECLARE_INTERFACE(QGenericPluginFactoryInterface, QGenericPluginFactoryInterface_iid)

class Q_GUI_EXPORT QGenericPlugin : public QObject, public QGenericPluginFactoryInterface
{
    CS_OBJECT(QGenericPlugin)
    CS_INTERFACES(QGenericPluginFactoryInterface, QFactoryInterface)

   public:
       explicit QGenericPlugin(QObject *parent = 0);
       ~QGenericPlugin();
   
       virtual QStringList keys() const = 0;
       virtual QObject* create(const QString& name, const QString &spec) = 0;
};

QT_END_NAMESPACE

#endif // QGENERICPLUGIN_QPA_H
