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

#ifndef QDESIGNEREXPORTWIDGET_H
#define QDESIGNEREXPORTWIDGET_H

#include <QtCore/QtGlobal>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

#if 0
// pragma for syncqt, don't remove.
#pragma qt_class(QDesignerExportWidget)
#endif

#if defined(QDESIGNER_EXPORT_WIDGETS)
#  define QDESIGNER_WIDGET_EXPORT Q_DECL_EXPORT
#else
#  define QDESIGNER_WIDGET_EXPORT Q_DECL_IMPORT
#endif

QT_END_NAMESPACE

QT_END_HEADER

#endif //QDESIGNEREXPORTWIDGET_H
