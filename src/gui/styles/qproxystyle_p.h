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

#ifndef QPROXYSTYLE_P_H
#define QPROXYSTYLE_P_H

#include "qcommonstyle.h"
#include "qcommonstyle_p.h"
#include "qproxystyle.h"

#ifndef QT_NO_STYLE_PROXY

QT_BEGIN_NAMESPACE

class QProxyStylePrivate : public QCommonStylePrivate
{
    Q_DECLARE_PUBLIC(QProxyStyle)
public:
    void ensureBaseStyle() const;
private:
    QProxyStylePrivate() :
    QCommonStylePrivate(), baseStyle(0) {}
    mutable QPointer <QStyle> baseStyle;
};

QT_END_NAMESPACE

#endif // QT_NO_STYLE_PROXY

#endif //QPROXYSTYLE_P_H
