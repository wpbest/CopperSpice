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

#ifndef QGrayRaster_P_H
#define QGrayRaster_P_H

#ifdef __cplusplus
  extern "C" {
#endif

#include <qrasterdefs_p.h>
 
#ifndef QT_FT_EXPORT_VAR
#define QT_FT_EXPORT_VAR( x )  extern  x
#endif

/* Minimum buffer size for raster object, that accounts for TWorker and TCell sizes.*/
#define MINIMUM_POOL_SIZE 8192

QT_FT_EXPORT_VAR( const QT_FT_Raster_Funcs )  qt_ft_grays_raster;

#ifdef __cplusplus
  }
#endif

#endif

