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

#ifndef QSCRIPTEXTENSIONPLUGIN_H
#define QSCRIPTEXTENSIONPLUGIN_H

#include <QtCore/qplugin.h>
#include <QtScript/qscriptextensioninterface.h>

QT_BEGIN_NAMESPACE

class QScriptValue;

class Q_SCRIPT_EXPORT QScriptExtensionPlugin : public QObject,public QScriptExtensionInterface
{
    CS_OBJECT_MULTIPLE(QScriptExtensionPlugin, QObject)
    CS_INTERFACES(QScriptExtensionInterface, QFactoryInterface)

public:
    explicit QScriptExtensionPlugin(QObject *parent = 0);
    ~QScriptExtensionPlugin();

    virtual QStringList keys() const = 0;
    virtual void initialize(const QString &key, QScriptEngine *engine) = 0;

    QScriptValue setupPackage(const QString &key, QScriptEngine *engine) const;
};

QT_END_NAMESPACE

#endif
