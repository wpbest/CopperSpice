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

#ifndef QWINDOWSURFACE_DIRECTFB_H
#define QWINDOWSURFACE_DIRECTFB_H

#include <QtGui/private/qwindowsurface_p.h>
#include <private/qpixmap_blitter_p.h>
#include <directfb.h>

#include "qdirectfbconvenience.h"

QT_BEGIN_NAMESPACE

class QDirectFbWindowSurface : public QWindowSurface
{
public:
    QDirectFbWindowSurface(QWidget *window);

    QPaintDevice *paintDevice();
    void flush(QWidget *widget, const QRegion &region, const QPoint &offset);
    void resize (const QSize &size);
    bool scroll(const QRegion &area, int dx, int dy);

    void beginPaint(const QRegion &region);
    void endPaint(const QRegion &region);

private:
    void lockSurfaceToImage();

    QScopedPointer<QPixmap> m_pixmap;
    QBlittablePixmapData *m_pmdata;
    QDirectFBPointer<IDirectFBSurface> m_dfbSurface;
};

QT_END_NAMESPACE

#endif
