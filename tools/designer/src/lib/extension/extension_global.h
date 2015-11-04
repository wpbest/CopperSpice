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

#ifndef EXTENSION_GLOBAL_H
#define EXTENSION_GLOBAL_H

#include <QtCore/qglobal.h>

QT_BEGIN_HEADER
QT_BEGIN_NAMESPACE

#define QDESIGNER_EXTENSION_EXTERN Q_DECL_EXPORT
#define QDESIGNER_EXTENSION_IMPORT Q_DECL_IMPORT

#ifdef QT_DESIGNER_STATIC
#  define QDESIGNER_EXTENSION_EXPORT
#elif defined(QDESIGNER_EXTENSION_LIBRARY)
#  define QDESIGNER_EXTENSION_EXPORT QDESIGNER_EXTENSION_EXTERN
#else
#  define QDESIGNER_EXTENSION_EXPORT QDESIGNER_EXTENSION_IMPORT
#endif

QT_END_NAMESPACE
QT_END_HEADER

#endif // EXTENSION_GLOBAL_H
