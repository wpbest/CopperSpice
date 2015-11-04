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

#include "qpixmapfilter_vg_p.h"
#include "qvgimagepool_p.h"
#include <QtCore/qvarlengtharray.h>
#include <QtGui/qpainter.h>

QT_BEGIN_NAMESPACE

#if !defined(QT_SHIVAVG)

QVGPixmapConvolutionFilter::QVGPixmapConvolutionFilter()
    : QPixmapConvolutionFilter()
{
}

QVGPixmapConvolutionFilter::~QVGPixmapConvolutionFilter()
{
}

extern void qt_vg_drawVGImage
    (QPainter *painter, const QPointF& pos, VGImage vgImg);
extern void qt_vg_drawVGImageStencil
    (QPainter *painter, const QPointF& pos, VGImage vgImg, const QBrush& brush);

void QVGPixmapConvolutionFilter::draw
        (QPainter *painter, const QPointF &dest,
         const QPixmap &src, const QRectF &srcRect) const
{
    if (src.isNull())
        return;

    if (src.pixmapData()->classId() != QPixmapData::OpenVGClass) {
        // The pixmap data is not an instance of QVGPixmapData, so fall
        // back to the default convolution filter implementation.
        QPixmapConvolutionFilter::draw(painter, dest, src, srcRect);
        return;
    }

    QVGPixmapData *pd = static_cast<QVGPixmapData *>(src.pixmapData());

    VGImage srcImage = pd->toVGImage();
    if (srcImage == VG_INVALID_HANDLE)
        return;

    QSize size = pd->size();
    VGImage dstImage = QVGImagePool::instance()->createTemporaryImage
        (VG_sARGB_8888_PRE, size.width(), size.height(),
         VG_IMAGE_QUALITY_FASTER, pd);
    if (dstImage == VG_INVALID_HANDLE)
        return;

    int kernelWidth = rows();
    int kernelHeight = columns();
    const qreal *kern = convolutionKernel();
    QVarLengthArray<VGshort> kernel;
    for (int i = 0; i < kernelWidth; ++i) {
        for (int j = 0; j < kernelHeight; ++j) {
            kernel.append((VGshort)(kern[j * kernelWidth + i] * 1024.0f));
        }
    }

    VGfloat values[4];
    values[0] = 0.0f;
    values[1] = 0.0f;
    values[2] = 0.0f;
    values[3] = 0.0f;
    vgSetfv(VG_TILE_FILL_COLOR, 4, values);

    vgConvolve(dstImage, srcImage,
               kernelWidth, kernelHeight, 0, 0,
               kernel.constData(), 1.0f / 1024.0f, 0.0f,
               VG_TILE_FILL);

    VGImage child = VG_INVALID_HANDLE;

    if (srcRect.isNull() ||
        (srcRect.topLeft().isNull() && srcRect.size() == size)) {
        child = dstImage;
    } else {
        QRect src = srcRect.toRect();
        child = vgChildImage(dstImage, src.x(), src.y(), src.width(), src.height());
    }

    qt_vg_drawVGImage(painter, dest, child);

    if(child != dstImage)
        vgDestroyImage(child);
    QVGImagePool::instance()->releaseImage(0, dstImage);
}

QVGPixmapColorizeFilter::QVGPixmapColorizeFilter()
    : QPixmapColorizeFilter()
{
}

QVGPixmapColorizeFilter::~QVGPixmapColorizeFilter()
{
}

void QVGPixmapColorizeFilter::draw(QPainter *painter, const QPointF &dest, const QPixmap &src, const QRectF &srcRect) const
{
    if (src.isNull())
        return;

    if (src.pixmapData()->classId() != QPixmapData::OpenVGClass) {
        // The pixmap data is not an instance of QVGPixmapData, so fall
        // back to the default colorize filter implementation.
        QPixmapColorizeFilter::draw(painter, dest, src, srcRect);
        return;
    }

    QVGPixmapData *pd = static_cast<QVGPixmapData *>(src.pixmapData());

    VGImage srcImage = pd->toVGImage();
    if (srcImage == VG_INVALID_HANDLE)
        return;

    QSize size = pd->size();
    VGImage dstImage = QVGImagePool::instance()->createTemporaryImage
        (VG_sARGB_8888_PRE, size.width(), size.height(),
         VG_IMAGE_QUALITY_FASTER, pd);
    if (dstImage == VG_INVALID_HANDLE)
        return;

    // Determine the weights for the matrix from the color and strength.
    QColor c = color();
    VGfloat strength = this->strength();
    VGfloat weights[3];
    VGfloat invweights[3];
    VGfloat alpha = c.alphaF();
    weights[0] = c.redF() * alpha;
    weights[1] = c.greenF() * alpha;
    weights[2] = c.blueF() * alpha;
    invweights[0] = (1.0f - weights[0]) * strength;
    invweights[1] = (1.0f - weights[1]) * strength;
    invweights[2] = (1.0f - weights[2]) * strength;

    // Grayscale weights.
    static const VGfloat redGray = 11.0f / 32.0f;
    static const VGfloat greenGray = 16.0f / 32.0f;
    static const VGfloat blueGray = 1.0f - (redGray + greenGray);

    VGfloat matrix[5][4];
    matrix[0][0] = redGray * invweights[0] + (1.0f - strength);
    matrix[0][1] = redGray * invweights[1];
    matrix[0][2] = redGray * invweights[2];
    matrix[0][3] = 0.0f;
    matrix[1][0] = greenGray * invweights[0];
    matrix[1][1] = greenGray * invweights[1] + (1.0f - strength);
    matrix[1][2] = greenGray * invweights[2];
    matrix[1][3] = 0.0f;
    matrix[2][0] = blueGray * invweights[0];
    matrix[2][1] = blueGray * invweights[1];
    matrix[2][2] = blueGray * invweights[2] + (1.0f - strength);
    matrix[2][3] = 0.0f;
    matrix[3][0] = 0.0f;
    matrix[3][1] = 0.0f;
    matrix[3][2] = 0.0f;
    matrix[3][3] = 1.0f;
    matrix[4][0] = weights[0] * strength;
    matrix[4][1] = weights[1] * strength;
    matrix[4][2] = weights[2] * strength;
    matrix[4][3] = 0.0f;

    vgColorMatrix(dstImage, srcImage, matrix[0]);

    VGImage child = VG_INVALID_HANDLE;

    if (srcRect.isNull() ||
        (srcRect.topLeft().isNull() && srcRect.size() == size)) {
        child = dstImage;
    } else {
        QRect src = srcRect.toRect();
        child = vgChildImage(dstImage, src.x(), src.y(), src.width(), src.height());
    }

    qt_vg_drawVGImage(painter, dest, child);

    if(child != dstImage)
        vgDestroyImage(child);
    QVGImagePool::instance()->releaseImage(0, dstImage);
}

QVGPixmapDropShadowFilter::QVGPixmapDropShadowFilter()
    : QPixmapDropShadowFilter()
{
}

QVGPixmapDropShadowFilter::~QVGPixmapDropShadowFilter()
{
}

void QVGPixmapDropShadowFilter::draw(QPainter *painter, const QPointF &dest, const QPixmap &src, const QRectF &srcRect) const
{
    if (src.isNull())
        return;

    if (src.pixmapData()->classId() != QPixmapData::OpenVGClass) {
        // The pixmap data is not an instance of QVGPixmapData, so fall
        // back to the default drop shadow filter implementation.
        QPixmapDropShadowFilter::draw(painter, dest, src, srcRect);
        return;
    }

    QVGPixmapData *pd = static_cast<QVGPixmapData *>(src.pixmapData());

    VGImage srcImage = pd->toVGImage();
    if (srcImage == VG_INVALID_HANDLE)
        return;

    QSize size = pd->size();
    VGImage dstImage = QVGImagePool::instance()->createTemporaryImage
        (VG_A_8, size.width(), size.height(),
         VG_IMAGE_QUALITY_FASTER, pd);
    if (dstImage == VG_INVALID_HANDLE)
        return;

    // Clamp the radius range.  We divide by 2 because the OpenVG blur
    // is "too blurry" compared to the default raster implementation.
    VGfloat maxRadius = VGfloat(vgGeti(VG_MAX_GAUSSIAN_STD_DEVIATION));
    VGfloat radiusF = VGfloat(blurRadius()) / 2.0f;
    if (radiusF < 0.001f)
        radiusF = 0.001f;
    else if (radiusF > maxRadius)
        radiusF = maxRadius;

    // Blur the blackened source image.
    vgGaussianBlur(dstImage, srcImage, radiusF, radiusF, VG_TILE_PAD);

    VGImage child = VG_INVALID_HANDLE;

    QRect srect;
    if (srcRect.isNull() ||
        (srcRect.topLeft().isNull() && srcRect.size() == size)) {
        child = dstImage;
        srect = QRect(0, 0, size.width(), size.height());
    } else {
        srect = srcRect.toRect();
        child = vgChildImage(dstImage, srect.x(), srect.y(), srect.width(), srect.height());
    }

    qt_vg_drawVGImageStencil(painter, dest + offset(), child, color());

    if(child != dstImage)
        vgDestroyImage(child);
    QVGImagePool::instance()->releaseImage(0, dstImage);

    // Now draw the actual pixmap over the top.
    painter->drawPixmap(dest, src, srect);
}

QVGPixmapBlurFilter::QVGPixmapBlurFilter(QObject *parent)
    : QPixmapBlurFilter(parent)
{
}

QVGPixmapBlurFilter::~QVGPixmapBlurFilter()
{
}

void QVGPixmapBlurFilter::draw(QPainter *painter, const QPointF &dest, const QPixmap &src, const QRectF &srcRect) const
{
    if (src.isNull())
        return;

    if (src.pixmapData()->classId() != QPixmapData::OpenVGClass) {
        // The pixmap data is not an instance of QVGPixmapData, so fall
        // back to the default blur filter implementation.
        QPixmapBlurFilter::draw(painter, dest, src, srcRect);
        return;
    }

    QVGPixmapData *pd = static_cast<QVGPixmapData *>(src.pixmapData());

    VGImage srcImage = pd->toVGImage();
    if (srcImage == VG_INVALID_HANDLE)
        return;

    QSize size = pd->size();
    VGImage dstImage = QVGImagePool::instance()->createTemporaryImage
        (VG_sARGB_8888_PRE, size.width(), size.height(),
         VG_IMAGE_QUALITY_FASTER, pd);
    if (dstImage == VG_INVALID_HANDLE)
        return;

    // Clamp the radius range.  We divide by 2 because the OpenVG blur
    // is "too blurry" compared to the default raster implementation.
    VGfloat maxRadius = VGfloat(vgGeti(VG_MAX_GAUSSIAN_STD_DEVIATION));
    VGfloat radiusF = VGfloat(radius()) / 2.0f;
    if (radiusF < 0.001f)
        radiusF = 0.001f;
    else if (radiusF > maxRadius)
        radiusF = maxRadius;

    vgGaussianBlur(dstImage, srcImage, radiusF, radiusF, VG_TILE_PAD);

    VGImage child = VG_INVALID_HANDLE;

    if (srcRect.isNull() ||
        (srcRect.topLeft().isNull() && srcRect.size() == size)) {
        child = dstImage;
    } else {
        QRect src = srcRect.toRect();
        child = vgChildImage(dstImage, src.x(), src.y(), src.width(), src.height());
    }

    qt_vg_drawVGImage(painter, dest, child);

    if(child != dstImage)
        vgDestroyImage(child);
    QVGImagePool::instance()->releaseImage(0, dstImage);
}

#endif

QT_END_NAMESPACE
