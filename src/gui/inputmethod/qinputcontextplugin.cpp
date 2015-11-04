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

#include "qinputcontextplugin.h"

#ifndef QT_NO_IM

QT_BEGIN_NAMESPACE

 
/*!
    Constructs a input context plugin with the given \a parent. This
    is invoked automatically by the Q_EXPORT_PLUGIN2() macro.
*/
QInputContextPlugin::QInputContextPlugin(QObject *parent)
    :QObject(parent)
{
}

/*!
    Destroys the input context plugin.

    You never have to call this explicitly. Qt destroys a plugin
    automatically when it's no longer used.
*/
QInputContextPlugin::~QInputContextPlugin()
{
}

QT_END_NAMESPACE

#endif // QT_NO_IM
