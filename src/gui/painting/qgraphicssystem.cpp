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

#include "qgraphicssystem_p.h"

#ifdef Q_WS_X11
# include <private/qpixmap_x11_p.h>
#endif
#if defined(Q_WS_WIN)
# include <private/qpixmap_raster_p.h>
#endif
#ifdef Q_WS_MAC
# include <private/qpixmap_mac_p.h>
#endif
#ifdef Q_WS_QPA
# include <QtGui/private/qapplication_p.h>
#endif

# include <private/qgraphicssystemex_p.h>

QT_BEGIN_NAMESPACE

QGraphicsSystem::~QGraphicsSystem()
{
}

QPixmapData *QGraphicsSystem::createDefaultPixmapData(QPixmapData::PixelType type)
{
#ifdef Q_WS_QWS
    Q_UNUSED(type);
#endif
#if defined(Q_WS_X11)
    return new QX11PixmapData(type);
#elif defined(Q_WS_WIN)
    return new QRasterPixmapData(type);
#elif defined(Q_WS_MAC)
    return new QMacPixmapData(type);
#elif defined(Q_WS_QPA)
    return QApplicationPrivate::platformIntegration()->createPixmapData(type);
#elif !defined(Q_WS_QWS)
#error QGraphicsSystem::createDefaultPixmapData() not implemented
#endif
    return 0;
}

QPixmapData *QGraphicsSystem::createPixmapData(QPixmapData *origin)
{
    return createPixmapData(origin->pixelType());
}

QGraphicsSystemEx* QGraphicsSystem::platformExtension()
{
    return 0;
}

QT_END_NAMESPACE
