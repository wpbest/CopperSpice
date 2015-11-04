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

#ifndef QSVGGENERATOR_H
#define QSVGGENERATOR_H

#include <QtGui/qpaintdevice.h>

#ifndef QT_NO_SVGGENERATOR

#include <QtCore/qnamespace.h>
#include <QtCore/qiodevice.h>
#include <QtCore/qscopedpointer.h>

QT_BEGIN_NAMESPACE

class QSvgGeneratorPrivate;

class Q_SVG_EXPORT QSvgGenerator : public QPaintDevice
{
    CS_GADGET(QSvgGenerator)
    Q_DECLARE_PRIVATE(QSvgGenerator)

    SVG_CS_PROPERTY_READ(size, size)
    SVG_CS_PROPERTY_WRITE(size, setSize)

    SVG_CS_PROPERTY_READ(viewBox, viewBoxF)
    SVG_CS_PROPERTY_WRITE(viewBox, cs_setViewBox)

    SVG_CS_PROPERTY_READ(title, title)
    SVG_CS_PROPERTY_WRITE(title, setTitle)

    SVG_CS_PROPERTY_READ(description, description)
    SVG_CS_PROPERTY_WRITE(description, setDescription)

    SVG_CS_PROPERTY_READ(fileName, fileName)
    SVG_CS_PROPERTY_WRITE(fileName, setFileName)

    SVG_CS_PROPERTY_READ(outputDevice, outputDevice)
    SVG_CS_PROPERTY_WRITE(outputDevice, setOutputDevice)

    SVG_CS_PROPERTY_READ(resolution, resolution)
    SVG_CS_PROPERTY_WRITE(resolution, setResolution)

public:
    QSvgGenerator();
    ~QSvgGenerator();

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    QSize size() const;
    void setSize(const QSize &size);

    QRect viewBox() const;
    QRectF viewBoxF() const;
    void setViewBox(const QRect &viewBox);
    void setViewBox(const QRectF &viewBox);

    // wrapper for overloaded method
    inline void cs_setViewBox(const QRectF &viewBox);
     
    QString fileName() const;
    void setFileName(const QString &fileName);

    QIODevice *outputDevice() const;
    void setOutputDevice(QIODevice *outputDevice);

    void setResolution(int dpi);
    int resolution() const;

protected:
    QPaintEngine *paintEngine() const;
    int metric(QPaintDevice::PaintDeviceMetric metric) const;

private:
    QScopedPointer<QSvgGeneratorPrivate> d_ptr;
};

void QSvgGenerator::cs_setViewBox(const QRectF &viewBox) 
{ 
   setViewBox(viewBox);
}

QT_END_NAMESPACE

#endif // QT_NO_SVGGENERATOR
#endif // QSVGGENERATOR_H
