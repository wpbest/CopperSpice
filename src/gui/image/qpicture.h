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

#ifndef QPICTURE_H
#define QPICTURE_H

#include <QtCore/qstringlist.h>
#include <QtCore/qsharedpointer.h>
#include <QtGui/qpaintdevice.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_PICTURE

class QPicturePrivate;

class Q_GUI_EXPORT QPicture : public QPaintDevice
{
    Q_DECLARE_PRIVATE(QPicture)

public:
    explicit QPicture(int formatVersion = -1);
    QPicture(const QPicture &);
    ~QPicture();

    bool isNull() const;

    int devType() const;
    uint size() const;
    const char* data() const;
    virtual void setData(const char* data, uint size);

    bool play(QPainter *p);

    bool load(QIODevice *dev, const char *format = 0);
    bool load(const QString &fileName, const char *format = 0);
    bool save(QIODevice *dev, const char *format = 0);
    bool save(const QString &fileName, const char *format = 0);

    QRect boundingRect() const;
    void setBoundingRect(const QRect &r);

    QPicture& operator=(const QPicture &p);

    inline QPicture &operator=(QPicture &&other)
    { qSwap(d_ptr, other.d_ptr); return *this; }

    inline void swap(QPicture &other) { d_ptr.swap(other.d_ptr); }
    void detach();
    bool isDetached() const;

    friend Q_GUI_EXPORT QDataStream &operator<<(QDataStream &in, const QPicture &p);
    friend Q_GUI_EXPORT QDataStream &operator>>(QDataStream &in, QPicture &p);

    static const char* pictureFormat(const QString &fileName);
    static QList<QByteArray> inputFormats();
    static QList<QByteArray> outputFormats();
    static QStringList inputFormatList();
    static QStringList outputFormatList();

    QPaintEngine *paintEngine() const;

    typedef QExplicitlySharedDataPointer<QPicturePrivate> DataPtr;
    inline DataPtr &data_ptr() { return d_ptr; }

protected:
    QPicture(QPicturePrivate &data);

    int metric(PaintDeviceMetric m) const;

private:
    bool exec(QPainter *p, QDataStream &ds, int i);
    void detach_helper();

    QExplicitlySharedDataPointer<QPicturePrivate> d_ptr;
    friend class QPicturePaintEngine;
    friend class QAlphaPaintEngine;
    friend class QPreviewPaintEngine;
   
};

Q_DECLARE_SHARED(QPicture)


#ifndef QT_NO_PICTUREIO
class QIODevice;
class QPictureIO;
typedef void (*picture_io_handler)(QPictureIO *); // picture IO handler

struct QPictureIOData;

class Q_GUI_EXPORT QPictureIO
{

public:
    QPictureIO();
    QPictureIO(QIODevice *ioDevice, const char *format);
    QPictureIO(const QString &fileName, const char *format);
    ~QPictureIO();

    const QPicture &picture() const;
    int status() const;
    const char *format() const;
    QIODevice *ioDevice() const;
    QString fileName() const;
    int quality() const;
    QString description() const;
    const char *parameters() const;
    float gamma() const;

    void setPicture(const QPicture &);
    void setStatus(int);
    void setFormat(const char *);
    void setIODevice(QIODevice *);
    void setFileName(const QString &);
    void setQuality(int);
    void setDescription(const QString &);
    void setParameters(const char *);
    void setGamma(float);

    bool read();
    bool write();

    static QByteArray pictureFormat(const QString &fileName);
    static QByteArray pictureFormat(QIODevice *);
    static QList<QByteArray> inputFormats();
    static QList<QByteArray> outputFormats();

    static void defineIOHandler(const char *format,
                                const char *header,
                                const char *flags,
                                picture_io_handler read_picture,
                                picture_io_handler write_picture);

private:
    Q_DISABLE_COPY(QPictureIO)

    void init();

    QPictureIOData *d;
};

#endif //QT_NO_PICTUREIO


/*****************************************************************************
  QPicture stream functions
 *****************************************************************************/

#ifndef QT_NO_DATASTREAM
Q_GUI_EXPORT QDataStream &operator<<(QDataStream &, const QPicture &);
Q_GUI_EXPORT QDataStream &operator>>(QDataStream &, QPicture &);
#endif

#endif // QT_NO_PICTURE

QT_END_NAMESPACE

#endif // QPICTURE_H
