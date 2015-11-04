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

#include "qdeclarativeextensionplugin.h"

QT_BEGIN_NAMESPACE

QDeclarativeExtensionPlugin::QDeclarativeExtensionPlugin(QObject *parent)
    : QObject(parent)
{
}

/*!
  \internal
 */
QDeclarativeExtensionPlugin::~QDeclarativeExtensionPlugin()
{
}

/*!
    \fn void QDeclarativeExtensionPlugin::initializeEngine(QDeclarativeEngine *engine, const char *uri)

    Initializes the extension from the \a uri using the \a engine. Here an application
    plugin might, for example, expose some data or objects to QML,
    as context properties on the engine's root context.
*/

void QDeclarativeExtensionPlugin::initializeEngine(QDeclarativeEngine *engine, const char *uri)
{
    Q_UNUSED(engine);
    Q_UNUSED(uri);
}

QT_END_NAMESPACE
