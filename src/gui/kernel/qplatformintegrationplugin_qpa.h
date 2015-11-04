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

#ifndef QPLATFORMINTEGRATIONPLUGIN_QPA_H
#define QPLATFORMINTEGRATIONPLUGIN_QPA_H

#include <QtCore/qplugin.h>
#include <QtCore/qfactoryinterface.h>

QT_BEGIN_NAMESPACE

class QPlatformIntegration;

struct QPlatformIntegrationFactoryInterface : public QFactoryInterface
{
    virtual QPlatformIntegration *create(const QString &key, const QStringList &paramList) = 0;
};

#define QPlatformIntegrationFactoryInterface_iid "com.copperspice.QPlatformIntegrationFactoryInterface"

CS_DECLARE_INTERFACE(QPlatformIntegrationFactoryInterface, QPlatformIntegrationFactoryInterface_iid)

class Q_GUI_EXPORT QPlatformIntegrationPlugin : public QObject, public QPlatformIntegrationFactoryInterface
{
    CS_OBJECT_MULTIPLE(QPlatformIntegrationPlugin, QObject)
    CS_INTERFACES(QPlatformIntegrationFactoryInterface, QFactoryInterface)

   public:
       explicit QPlatformIntegrationPlugin(QObject *parent = 0);
       ~QPlatformIntegrationPlugin();
   
       virtual QStringList keys() const = 0;
       virtual QPlatformIntegration *create(const QString &key, const QStringList &paramList) = 0;
};

QT_END_NAMESPACE

#endif // QPLATFORMINTEGRATIONPLUGIN_H
