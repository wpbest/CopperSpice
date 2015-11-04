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

#ifndef QICOHANDLER_P_H
#define QICOHANDLER_P_H

#include <QtGui/QImageIOHandler>

QT_BEGIN_NAMESPACE

class ICOReader;
class QIcoHandler: public QImageIOHandler
{
public:
    QIcoHandler();
    virtual ~QIcoHandler();

    bool canRead() const;
    bool read(QImage *image);
    bool write(const QImage &image);

    QByteArray name() const;
    
    int imageCount() const;
    bool jumpToImage(int imageNumber);
    bool jumpToNextImage();
    
    static bool canRead(QIODevice *device);
    
    bool supportsOption(ImageOption option) const;
    QVariant option(ImageOption option) const;

private:
    
    void setupReader() const;

    int m_currentIconIndex;
    mutable ICOReader *m_ICOReader;

};

QT_END_NAMESPACE

#endif /* QTICOHANDLER_H */

