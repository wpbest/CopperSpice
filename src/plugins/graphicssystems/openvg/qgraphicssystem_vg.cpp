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

#include "qgraphicssystem_vg_p.h"
#include <QtOpenVG/private/qpixmapdata_vg_p.h>
#include <QtOpenVG/private/qwindowsurface_vg_p.h>
#include <QtOpenVG/private/qvgimagepool_p.h>

#include <QtGui/private/qapplication_p.h>

QT_BEGIN_NAMESPACE

QVGGraphicsSystem::QVGGraphicsSystem()
{
    QApplicationPrivate::graphics_system_name = QLatin1String("openvg");
}

QPixmapData *QVGGraphicsSystem::createPixmapData(QPixmapData::PixelType type) const
{
#if !defined(QVG_NO_SINGLE_CONTEXT) && !defined(QVG_NO_PIXMAP_DATA)
    // Pixmaps can use QVGPixmapData; bitmaps must use raster.
    if (type == QPixmapData::PixmapType)
        return new QVGPixmapData(type);
    else
        return new QRasterPixmapData(type);
#else
    return new QRasterPixmapData(type);
#endif
}

QWindowSurface *QVGGraphicsSystem::createWindowSurface(QWidget *widget) const
{
    return new QVGWindowSurface(widget);
}

QT_END_NAMESPACE
