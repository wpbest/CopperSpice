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

#ifndef QPAINTENGINE_X11_P_H
#define QPAINTENGINE_X11_P_H

#include "QtGui/qpaintengine.h"
#include "QtGui/qregion.h"
#include "QtGui/qpen.h"
#include "QtCore/qpoint.h"
#include "qpaintengine_p.h"
#include "qpainter_p.h"
#include "qpolygonclipper_p.h"

typedef unsigned long Picture;

QT_BEGIN_NAMESPACE

class QX11PaintEnginePrivate;
class QFontEngineFT;
class QXRenderTessellator;

struct qt_float_point
{
    qreal x, y;
};

class QX11PaintEngine : public QPaintEngine
{
    Q_DECLARE_PRIVATE(QX11PaintEngine)
public:
    QX11PaintEngine();
    ~QX11PaintEngine();

    bool begin(QPaintDevice *pdev);
    bool end();

    void updateState(const QPaintEngineState &state);

    void updatePen(const QPen &pen);
    void updateBrush(const QBrush &brush, const QPointF &pt);
    void updateRenderHints(QPainter::RenderHints hints);
    void updateFont(const QFont &font);
    void updateMatrix(const QTransform &matrix);
    void updateClipRegion_dev(const QRegion &region, Qt::ClipOperation op);

    void drawLines(const QLine *lines, int lineCount);
    void drawLines(const QLineF *lines, int lineCount);

    void drawRects(const QRect *rects, int rectCount);
    void drawRects(const QRectF *rects, int rectCount);

    void drawPoints(const QPoint *points, int pointCount);
    void drawPoints(const QPointF *points, int pointCount);

    void drawEllipse(const QRect &r);
    void drawEllipse(const QRectF &r);

    virtual void drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode);
    inline void drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode)
        { QPaintEngine::drawPolygon(points, pointCount, mode); }

    void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr);
    void drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s);
    void drawPath(const QPainterPath &path);
    void drawTextItem(const QPointF &p, const QTextItem &textItem);
    void drawImage(const QRectF &r, const QImage &img, const QRectF &sr,
                   Qt::ImageConversionFlags flags = Qt::AutoColor);

    virtual Qt::HANDLE handle() const;
    inline Type type() const { return QPaintEngine::X11; }

    QPainter::RenderHints supportedRenderHints() const;

protected:
    QX11PaintEngine(QX11PaintEnginePrivate &dptr);

    void drawXLFD(const QPointF &p, const QTextItemInt &si);
#ifndef QT_NO_FONTCONFIG
    void drawFreetype(const QPointF &p, const QTextItemInt &si);
#endif

    friend class QPixmap;
    friend class QFontEngineBox;
    friend Q_GUI_EXPORT GC qt_x11_get_pen_gc(QPainter *);
    friend Q_GUI_EXPORT GC qt_x11_get_brush_gc(QPainter *);

private:
    Q_DISABLE_COPY(QX11PaintEngine)
};

class QX11PaintEnginePrivate : public QPaintEnginePrivate
{
    Q_DECLARE_PUBLIC(QX11PaintEngine)
public:
    QX11PaintEnginePrivate()
    {
        scrn = -1;
        hd = 0;
        picture = 0;
        gc = gc_brush = 0;
        dpy  = 0;
        xinfo = 0;
        txop = QTransform::TxNone;
        has_clipping = false;
        render_hints = 0;
        xform_scale = 1;
#ifndef QT_NO_XRENDER
        tessellator = 0;
#endif
    }
    enum GCMode {
        PenGC,
        BrushGC
    };

    void init();
    void fillPolygon_translated(const QPointF *points, int pointCount, GCMode gcMode,
                                QPaintEngine::PolygonDrawMode mode);
    void fillPolygon_dev(const QPointF *points, int pointCount, GCMode gcMode,
                         QPaintEngine::PolygonDrawMode mode);
    void fillPath(const QPainterPath &path, GCMode gcmode, bool transform);
    void strokePolygon_dev(const QPointF *points, int pointCount, bool close);
    void strokePolygon_translated(const QPointF *points, int pointCount, bool close);
    void setupAdaptedOrigin(const QPoint &p);
    void resetAdaptedOrigin();
    void decidePathFallback() {
        use_path_fallback = has_alpha_brush
                            || has_alpha_pen
                            || has_custom_pen
                            || has_complex_xform
                            || (render_hints & QPainter::Antialiasing);
    }
    void decideCoordAdjust() {
        adjust_coords = !(render_hints & QPainter::Antialiasing)
                        && (has_alpha_pen
                            || (has_alpha_brush && has_pen && !has_alpha_pen)
                            || (cpen.style() > Qt::SolidLine));
    }
    void clipPolygon_dev(const QPolygonF &poly, QPolygonF *clipped_poly);
    void systemStateChanged();

    Display *dpy;
    int scrn;
    int pdev_depth;
    Qt::HANDLE hd;
    QPixmap brush_pm;
#if !defined (QT_NO_XRENDER)
    Qt::HANDLE picture;
    Qt::HANDLE current_brush;
    QPixmap bitmap_texture;
    int composition_mode;
#else
    Qt::HANDLE picture;
#endif
    GC gc;
    GC gc_brush;

    QPen cpen;
    QBrush cbrush;
    QRegion crgn;
    QTransform matrix;
    qreal opacity;

    uint has_complex_xform : 1;
    uint has_scaling_xform : 1;
    uint has_non_scaling_xform : 1;
    uint has_custom_pen : 1;
    uint use_path_fallback : 1;
    uint adjust_coords : 1;
    uint has_clipping : 1;
    uint adapted_brush_origin : 1;
    uint adapted_pen_origin : 1;
    uint has_pen : 1;
    uint has_brush : 1;
    uint has_texture : 1;
    uint has_alpha_texture : 1;
    uint has_pattern : 1;
    uint has_alpha_pen : 1;
    uint has_alpha_brush : 1;
    uint render_hints;

    const QX11Info *xinfo;
    QPointF bg_origin;
    QTransform::TransformationType txop;
    qreal xform_scale;
    QPolygonClipper<qt_float_point, qt_float_point, float> polygonClipper;

    int xlibMaxLinePoints;
#ifndef QT_NO_XRENDER
    QXRenderTessellator *tessellator;
#endif
};

QT_END_NAMESPACE

#endif // QPAINTENGINE_X11_P_H
