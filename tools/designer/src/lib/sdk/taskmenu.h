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

#ifndef TASKMENU_H
#define TASKMENU_H

#include <QtDesigner/extension.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QAction;

class QDesignerTaskMenuExtension
{
public:
    virtual ~QDesignerTaskMenuExtension() {}

    virtual QAction *preferredEditAction() const;

    virtual QList<QAction*> taskActions() const = 0;
};
Q_DECLARE_EXTENSION_INTERFACE(QDesignerTaskMenuExtension, "com.trolltech.Qt.Designer.TaskMenu")


inline QAction *QDesignerTaskMenuExtension::preferredEditAction() const
{ return 0; }

QT_END_NAMESPACE

QT_END_HEADER

#endif // TASKMENU_H
