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

/******************************************************************
** Copyright (C) 2006-2008 Ricardo Villalba <rvm@escomposlinux.org>
******************************************************************/

#ifndef PHONON_SWIFTSLIDER_P_H
#define PHONON_SWIFTSLIDER_P_H

#include <QtGui/QSlider>

QT_BEGIN_NAMESPACE

#if !defined(QT_NO_PHONON_SEEKSLIDER) && !defined(QT_NO_PHONON_VOLUMESLIDER)

namespace Phonon
{

class SwiftSlider : public QSlider
{
	CS_OBJECT(SwiftSlider)
public:
	SwiftSlider(Qt::Orientation orientation, QWidget * parent);
	~SwiftSlider();

private:
	void mousePressEvent(QMouseEvent *event);
	inline int pick(const QPoint &pt) const;
	int pixelPosToRangeValue(int pos) const;
};

} // namespace Phonon

#endif //QT_NO_PHONON_VOLUMESLIDER && QT_NO_PHONON_VOLUMESLIDER

QT_END_NAMESPACE

#endif //SWIFTSLIDER_H
