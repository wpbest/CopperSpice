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

#ifndef QGRAPHICSSVGITEM_H
#define QGRAPHICSSVGITEM_H

#include <QtGui/qgraphicsitem.h>

#ifndef QT_NO_GRAPHICSSVGITEM

QT_BEGIN_NAMESPACE

class QSvgRenderer;
class QGraphicsSvgItemPrivate;

class Q_SVG_EXPORT QGraphicsSvgItem : public QGraphicsObject
{
    CS_OBJECT(QGraphicsSvgItem)
    CS_INTERFACES(QGraphicsItem)

    SVG_CS_PROPERTY_READ(elementId, elementId)
    SVG_CS_PROPERTY_WRITE(elementId, setElementId)
    SVG_CS_PROPERTY_READ(maximumCacheSize, maximumCacheSize)
    SVG_CS_PROPERTY_WRITE(maximumCacheSize, setMaximumCacheSize)

public:
    QGraphicsSvgItem(QGraphicsItem *parentItem=0);
    QGraphicsSvgItem(const QString &fileName, QGraphicsItem *parentItem=0);

    void setSharedRenderer(QSvgRenderer *renderer);
    QSvgRenderer *renderer() const;

    void setElementId(const QString &id);
    QString elementId() const;

    void setCachingEnabled(bool);
    bool isCachingEnabled() const;

    void setMaximumCacheSize(const QSize &size);
    QSize maximumCacheSize() const;

    virtual QRectF boundingRect() const;

    virtual void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget=0);

    enum { Type = 13 };
    virtual int type() const;

private:
    Q_DISABLE_COPY(QGraphicsSvgItem)
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), QGraphicsSvgItem)

    SVG_CS_SLOT_1(Private, void _q_repaintItem())
    SVG_CS_SLOT_2(_q_repaintItem)
};

QT_END_NAMESPACE

#endif // QT_NO_GRAPHICSSVGITEM
#endif // QGRAPHICSSVGITEM_H
