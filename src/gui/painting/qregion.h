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

#ifndef QREGION_H
#define QREGION_H

#include <QtCore/qatomic.h>
#include <QtCore/qrect.h>
#include <QtGui/qwindowdefs.h>

#ifndef QT_NO_DATASTREAM
#include <QtCore/qdatastream.h>
#endif

QT_BEGIN_NAMESPACE

template <class T> class QVector;
class QVariant;

#if defined(Q_WS_QWS) || defined(Q_WS_QPA) || defined(Q_WS_X11) || defined(Q_WS_MAC) || defined(Q_WS_WIN)
struct QRegionPrivate;
#endif

class QBitmap;

class Q_GUI_EXPORT QRegion
{
public:
    enum RegionType { Rectangle, Ellipse };

    QRegion();
    QRegion(int x, int y, int w, int h, RegionType t = Rectangle);
    QRegion(const QRect &r, RegionType t = Rectangle);
    QRegion(const QPolygon &pa, Qt::FillRule fillRule = Qt::OddEvenFill);

    QRegion(const QRegion &region);
    QRegion(const QBitmap &bitmap);
    ~QRegion();
    QRegion &operator=(const QRegion &);

    inline QRegion &operator=(QRegion &&other)
    { qSwap(d, other.d); return *this; }

    inline void swap(QRegion &other) { qSwap(d, other.d); }

    bool isEmpty() const;

    bool contains(const QPoint &p) const;
    bool contains(const QRect &r) const;

    void translate(int dx, int dy);
    inline void translate(const QPoint &p) { translate(p.x(), p.y()); }
    QRegion translated(int dx, int dy) const;
    inline QRegion translated(const QPoint &p) const { return translated(p.x(), p.y()); }

    // ### Qt5 - make these four functions QT4_SUPPORT
    QRegion unite(const QRegion &r) const;
    QRegion unite(const QRect &r) const;
    QRegion intersect(const QRegion &r) const;
    QRegion intersect(const QRect &r) const;
    QRegion subtract(const QRegion &r) const;
    QRegion eor(const QRegion &r) const;

    inline QRegion united(const QRegion &r) const { return unite(r); }
    inline QRegion united(const QRect &r) const { return unite(r); }
    inline QRegion intersected(const QRegion &r) const { return intersect(r); }
    inline QRegion intersected(const QRect &r) const { return intersect(r); }
    inline QRegion subtracted(const QRegion &r) const { return subtract(r); }
    inline QRegion xored(const QRegion &r) const { return eor(r); }

    bool intersects(const QRegion &r) const;
    bool intersects(const QRect &r) const;

    QRect boundingRect() const;
    QVector<QRect> rects() const;
    void setRects(const QRect *rect, int num);

#ifdef QT_DEPRECATED
    QT_DEPRECATED int numRects() const;
#endif

    int rectCount() const;

    const QRegion operator|(const QRegion &r) const;
    const QRegion operator+(const QRegion &r) const;
    const QRegion operator+(const QRect &r) const;
    const QRegion operator&(const QRegion &r) const;
    const QRegion operator&(const QRect &r) const;
    const QRegion operator-(const QRegion &r) const;
    const QRegion operator^(const QRegion &r) const;
    QRegion& operator|=(const QRegion &r);
    QRegion& operator+=(const QRegion &r);
    QRegion& operator+=(const QRect &r);
    QRegion& operator&=(const QRegion &r);
    QRegion& operator&=(const QRect &r);
    QRegion& operator-=(const QRegion &r);
    QRegion& operator^=(const QRegion &r);

    bool operator==(const QRegion &r) const;
    inline bool operator!=(const QRegion &r) const { return !(operator==(r)); }
    operator QVariant() const;

#if defined(Q_WS_WIN)
    inline HRGN    handle() const { ensureHandle(); return d->rgn; }

#elif defined(Q_WS_X11)
    inline Region handle() const { if(!d->rgn) updateX11Region(); return d->rgn; }

#elif defined(Q_WS_MAC)

#if defined Q_WS_MAC32
    RgnHandle toQDRgn() const;
    RgnHandle toQDRgnForUpdate_sys() const;
    static QRegion fromQDRgn(RgnHandle shape);
#endif

#ifdef QT_MAC_USE_COCOA
    inline HIMutableShapeRef handle(bool unused = false) const
    { Q_UNUSED(unused); return toHIMutableShape(); }
#else
    inline RgnHandle handle() const { return handle(false); }
    inline RgnHandle handle(bool) const { return toQDRgn(); }
#endif
    HIMutableShapeRef toHIMutableShape() const;
    static QRegion fromHIShapeRef(HIShapeRef shape);
#elif defined(Q_WS_QWS) || defined(Q_WS_QPA)
    inline void *handle() const { return d->qt_rgn; }
#endif

#ifndef QT_NO_DATASTREAM
    friend Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QRegion &);
    friend Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QRegion &);
#endif

private:
    QRegion copy() const;   // helper of detach
    void detach();

#if defined(Q_WS_WIN)
    void ensureHandle() const;
    QRegion winCombine(const QRegion &r, int num) const;
#elif defined(Q_WS_X11)
    void updateX11Region() const;
    void *clipRectangles(int &num) const;
    friend void *qt_getClipRects(const QRegion &r, int &num);
#elif defined(Q_WS_MAC)
    static OSStatus shape2QRegionHelper(int inMessage, HIShapeRef inShape, const CGRect *inRect, void *inRefcon);
#endif

    friend bool qt_region_strictContains(const QRegion &region, const QRect &rect);
    friend struct QRegionPrivate;

#ifndef QT_NO_DATASTREAM
    void exec(const QByteArray &ba, int ver = 0, QDataStream::ByteOrder byteOrder = QDataStream::BigEndian);
#endif

    struct QRegionData {
        QBasicAtomicInt ref;

#if defined(Q_WS_WIN)
        HRGN   rgn;
#elif defined(Q_WS_X11)
        Region rgn;
        void *xrectangles;
#endif

#if defined(Q_WS_QWS) || defined(Q_WS_QPA) || defined(Q_WS_X11) || defined(Q_WS_MAC) || defined(Q_WS_WIN)
        QRegionPrivate *qt_rgn;
#endif

    };

#if defined(Q_WS_WIN)
    friend class QETWidget;
#endif

    struct QRegionData *d;
    static struct QRegionData shared_empty;
    static void cleanUp(QRegionData *x);
};

/*****************************************************************************
  QRegion stream functions
 *****************************************************************************/

#ifndef QT_NO_DATASTREAM
Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QRegion &);
Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QRegion &);
#endif

#ifndef QT_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug, const QRegion &);
#endif

QT_END_NAMESPACE

#endif // QREGION_H
