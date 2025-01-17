/***********************************************************************
*
* Copyright (c) 2012-2013 Barbara Geller
* Copyright (c) 2012-2013 Ansel Sermersheim
* Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "qvfbmmap.h"
#include "qvfbhdr.h"

#include <QTimer>

#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>

QT_BEGIN_NAMESPACE

QMMapViewProtocol::QMMapViewProtocol(int displayid, const QSize &s,
                                     int d, QObject *parent)
    : QVFbViewProtocol(displayid, parent), hdr(0), dataCache(0), windowId(0)
{
    switch (d) {
    case 1:
    case 4:
    case 8:
    case 12:
    case 15:
    case 16:
    case 18:
    case 24:
    case 32:
        break;
    default:
        qFatal("Unsupported bit depth %d\n", d);
    }

    fileName = QString("/tmp/.qtvfb_map-%1").arg(displayid);

    int w = s.width();
    int h = s.height();


    kh = new QVFbKeyPipeProtocol(displayid);
    mh = new QVFbMouseLinuxTP(displayid);

    int bpl;
    if (d < 8)
	bpl = (w * d + 7) / 8;
    else
        bpl = w * ((d + 7) / 8);

    displaySize = bpl * h;

    unsigned char *data;
    uint data_offset_value = sizeof(QVFbHeader);
    const int page_size = getpagesize();
    if (data_offset_value % page_size)
        data_offset_value += page_size - (data_offset_value % page_size);

    dataSize = bpl * h + data_offset_value;

    unlink(fileName.toLocal8Bit().data());
    fd = ::open( fileName.toLocal8Bit().data(), O_CREAT|O_RDWR, 0666 );
    ::lseek(fd, dataSize, SEEK_SET);
    ::write(fd, "\0", 1);
    if (fd < 0) {
        data = (unsigned char *)-1;
    } else {
        // might need to do something about size?
        data = (unsigned char *)mmap(NULL, dataSize, PROT_WRITE | PROT_READ,
                                     MAP_SHARED, fd, 0);
        if (data == MAP_FAILED)
            data = (unsigned char *)-1;
    }

    if ( (long)data == -1 ){
        delete kh;
        delete mh;
	qFatal( "Cannot attach to mapped file %s", fileName.toLocal8Bit().data());
    }
    dataCache = (unsigned char *)malloc(displaySize);
    memset(dataCache, 0, displaySize);
    memset(data+sizeof(QVFbHeader), 0, displaySize);

    hdr = (QVFbHeader *)data;
    hdr->width = w;
    hdr->height = h;
    hdr->depth = d;
    hdr->linestep = bpl;
    hdr->numcols = 0;
    hdr->dataoffset = data_offset_value;
    hdr->update = QRect();
    hdr->brightness = 255;
    hdr->windowId = 0;

    mRefreshTimer = new QTimer(this);
    connect(mRefreshTimer, SIGNAL(timeout()), this, SLOT(flushChanges()));
}

QMMapViewProtocol::~QMMapViewProtocol()
{
    munmap((char *)hdr, dataSize);
    ::close(fd);
    unlink(fileName.toLocal8Bit().constData());
    free(dataCache);
    delete kh;
    delete mh;
}

int QMMapViewProtocol::brightness() const
{
    return hdr->brightness;
}

int QMMapViewProtocol::width() const
{
    return hdr->width;
}

int QMMapViewProtocol::height() const
{
    return hdr->height;
}

int QMMapViewProtocol::depth() const
{
    return hdr->depth;
}

int QMMapViewProtocol::linestep() const
{
    return hdr->linestep;
}

int  QMMapViewProtocol::numcols() const
{
    return hdr->numcols;
}

QVector<QRgb> QMMapViewProtocol::clut() const
{
    QVector<QRgb> vector(hdr->numcols);
    for (int i=0; i < hdr->numcols; ++i)
        vector[i] = hdr->clut[i];

    return vector;
}

unsigned char *QMMapViewProtocol::data() const
{
    return dataCache;
    //return ((unsigned char *)hdr)+hdr->dataoffset;
}

void QMMapViewProtocol::flushChanges()
{
    // based of dirty rect, copy changes from hdr to hdrcopy
    memcpy(dataCache, ((char *)hdr) + hdr->dataoffset, displaySize);
    emit displayDataChanged(QRect(0, 0, width(), height()));
}

void QMMapViewProtocol::setRate(int interval)
{
    if (interval > 0)
        return mRefreshTimer->start(1000/interval);
    else
        mRefreshTimer->stop();
}

int QMMapViewProtocol::rate() const
{
    int i = mRefreshTimer->interval();
    if (i > 0)
        return 1000/i;
    else
        return 0;
}

QT_END_NAMESPACE
