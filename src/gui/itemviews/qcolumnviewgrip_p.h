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

#ifndef QCOLUMNVIEWGRIP_P_H
#define QCOLUMNVIEWGRIP_P_H

#include <qwidget_p.h>

#ifndef QT_NO_QCOLUMNVIEW

QT_BEGIN_NAMESPACE

class QColumnViewGripPrivate;

class QColumnViewGrip : public QWidget
{
   CS_OBJECT(QColumnViewGrip)

public:
    GUI_CS_SIGNAL_1(Public, void gripMoved(int offset))
    GUI_CS_SIGNAL_2(gripMoved,offset) 

    explicit QColumnViewGrip(QWidget *parent = 0);
    ~QColumnViewGrip();
    int moveGrip(int offset);

protected:
    QColumnViewGrip(QColumnViewGripPrivate &, QWidget *parent = 0, Qt::WFlags f = 0);
    void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    Q_DECLARE_PRIVATE(QColumnViewGrip)
    Q_DISABLE_COPY(QColumnViewGrip)
};

class QColumnViewGripPrivate : public QWidgetPrivate
{
    Q_DECLARE_PUBLIC(QColumnViewGrip)

public:
    QColumnViewGripPrivate();
    ~QColumnViewGripPrivate() {}

    int originalXLocation;
};

QT_END_NAMESPACE

#endif // QT_NO_QCOLUMNVIEW

#endif //QCOLUMNVIEWGRIP_P_H
