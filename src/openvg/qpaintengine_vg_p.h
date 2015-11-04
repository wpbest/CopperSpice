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

#ifndef QPAINTENGINE_VG_P_H
#define QPAINTENGINE_VG_P_H

#include <qpaintengineex_p.h>
#include <qtextureglyphcache_p.h>

QT_BEGIN_NAMESPACE

struct QFixedPoint;
class QVGPaintEnginePrivate;
class QPixmapData;
class QVGEGLWindowSurfacePrivate;

class Q_OPENVG_EXPORT QVGPainterState : public QPainterState
{
public:
    QVGPainterState(QVGPainterState& other);
    QVGPainterState();
    ~QVGPainterState();

    bool isNew;
    QRegion clipRegion;
    QPaintEngine::DirtyFlags savedDirty;
};

class Q_OPENVG_EXPORT QVGPaintEngine : public QPaintEngineEx
{
    Q_DECLARE_PRIVATE(QVGPaintEngine)
public:
    QVGPaintEngine();
    ~QVGPaintEngine();

    Type type() const { return OpenVG; }

    QPainterState *createState(QPainterState *orig) const;

    bool begin(QPaintDevice *pdev);
    bool end();

    void draw(const QVectorPath &path);
    void fill(const QVectorPath &path, const QBrush &brush);
    void stroke(const QVectorPath &path, const QPen &pen);

    void clip(const QVectorPath &path, Qt::ClipOperation op);
    void clip(const QRect &rect, Qt::ClipOperation op);
    void clip(const QRegion &region, Qt::ClipOperation op);
    void clip(const QPainterPath &path, Qt::ClipOperation op);

    void clipEnabledChanged();
    void penChanged();
    void brushChanged();
    void brushOriginChanged();
    void opacityChanged();
    void compositionModeChanged();
    void renderHintsChanged();
    void transformChanged();

    void fillRect(const QRectF &rect, const QBrush &brush);
    void fillRect(const QRectF &rect, const QColor &color);

    void drawRoundedRect(const QRectF &rect, qreal xrad, qreal yrad, Qt::SizeMode mode);

    void drawRects(const QRect *rects, int rectCount);
    void drawRects(const QRectF *rects, int rectCount);

    void drawLines(const QLine *lines, int lineCount);
    void drawLines(const QLineF *lines, int lineCount);

    void drawEllipse(const QRectF &r);
    void drawEllipse(const QRect &r);

    void drawPath(const QPainterPath &path);

    void drawPoints(const QPointF *points, int pointCount);
    void drawPoints(const QPoint *points, int pointCount);

    void drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode);
    void drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode);

    void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr);
    void drawPixmap(const QPointF &pos, const QPixmap &pm);

    void drawImage(const QRectF &r, const QImage &pm, const QRectF &sr,
                   Qt::ImageConversionFlags flags = Qt::AutoColor);
    void drawImage(const QPointF &pos, const QImage &image);

    void drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s);

    void drawPixmapFragments(const QPainter::PixmapFragment *drawingData, int dataCount, const QPixmap &pixmap,
                             QFlags<QPainter::PixmapFragmentHint> hints);

    void drawTextItem(const QPointF &p, const QTextItem &textItem);
    void drawStaticTextItem(QStaticTextItem *staticTextItem);
    bool drawCachedGlyphs(int numGlyphs, const glyph_t *glyphs, const QFont &font,
                          QFontEngine *fontEngine, const QPointF &p,
                          const QFixedPoint *positions);

    void setState(QPainterState *s);
    QVGPainterState *state() { return static_cast<QVGPainterState *>(QPaintEngineEx::state()); }
    const QVGPainterState *state() const { return static_cast<const QVGPainterState *>(QPaintEngineEx::state()); }

    void beginNativePainting();
    void endNativePainting();

    QPixmapFilter *pixmapFilter(int type, const QPixmapFilter *prototype);

    QVGPaintEnginePrivate *vgPrivate() { Q_D(QVGPaintEngine); return d; }

    void fillRegion(const QRegion& region, const QColor& color, const QSize& surfaceSize);
    bool supportsTransformations(qreal, const QTransform &) const { return true; }

protected:
    QVGPaintEngine(QVGPaintEnginePrivate &data);

private:
    void restoreState(QPaintEngine::DirtyFlags dirty);
    void updateScissor();
    QRegion defaultClipRegion();
    bool isDefaultClipRegion(const QRegion& region);
    bool isDefaultClipRect(const QRect& rect);
    bool clearRect(const QRectF &rect, const QColor &color);
    bool canVgWritePixels(const QImage &image) const;
};

QT_END_NAMESPACE

#endif
