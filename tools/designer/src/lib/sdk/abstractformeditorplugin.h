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

#ifndef ABSTRACTFORMEDITORPLUGIN_H
#define ABSTRACTFORMEDITORPLUGIN_H

#include <QtDesigner/sdk_global.h>

#include <QtCore/QObject>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class QAction;

class QDESIGNER_SDK_EXPORT QDesignerFormEditorPluginInterface
{
public:
    virtual ~QDesignerFormEditorPluginInterface() {}

    virtual bool isInitialized() const = 0;
    virtual void initialize(QDesignerFormEditorInterface *core) = 0;
    virtual QAction *action() const = 0;

    virtual QDesignerFormEditorInterface *core() const = 0;
};
Q_DECLARE_INTERFACE(QDesignerFormEditorPluginInterface, "com.trolltech.Qt.Designer.QDesignerFormEditorPluginInterface")

QT_END_NAMESPACE

QT_END_HEADER

#endif // ABSTRACTFORMEDITORPLUGIN_H
