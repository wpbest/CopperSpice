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
#ifndef QVFB_SHM_PROTOCOL_H
#define QVFB_SHM_PROTOCOL_H

#include "qvfbprotocol.h"

QT_BEGIN_NAMESPACE

class QVFbHeader;
class QTimer;
class QShMemViewProtocol : public QVFbViewProtocol
{
    Q_OBJECT
public:
    QShMemViewProtocol(int display_id, const QSize &size, int depth, QObject *parent = 0);
    ~QShMemViewProtocol();

    int width() const;
    int height() const;
    int depth() const;
    int linestep() const;
    int  numcols() const;
    QVector<QRgb> clut() const;
    unsigned char *data() const;
    int brightness() const;

    void setRate(int);
    int rate() const;

protected slots:
    void flushChanges();

protected:
    QVFbKeyProtocol *keyHandler() const { return kh; }
    QVFbMouseProtocol *mouseHandler() const { return mh; }

private:
    QVFbKeyPipeProtocol *kh;
    QVFbMousePipe *mh;
    QVFbHeader *hdr;
    size_t displaySize;
    unsigned char *dataCache;
    int lockId;
    int shmId;
    QTimer *mRefreshTimer;
    WId windowId;
};

QT_END_NAMESPACE

#endif
