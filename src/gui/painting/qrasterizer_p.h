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

#ifndef QRASTERIZER_P_H
#define QRASTERIZER_P_H

#include "QtCore/qglobal.h"
#include "QtGui/qpainter.h"
#include <qdrawhelper_p.h>
#include <qrasterdefs_p.h>

QT_BEGIN_NAMESPACE

struct QSpanData;
class QRasterBuffer;
class QRasterizerPrivate;

class
#ifdef Q_WS_QWS
Q_GUI_EXPORT
#endif
QRasterizer
{
public:
    QRasterizer();
    ~QRasterizer();

    void setAntialiased(bool antialiased);
    void setClipRect(const QRect &clipRect);

    void initialize(ProcessSpans blend, void *data);

    void rasterize(const QT_FT_Outline *outline, Qt::FillRule fillRule);
    void rasterize(const QPainterPath &path, Qt::FillRule fillRule);

    // width should be in units of |a-b|
    void rasterizeLine(const QPointF &a, const QPointF &b, qreal width, bool squareCap = false);

private:
    QRasterizerPrivate *d;
};

QT_END_NAMESPACE

#endif
