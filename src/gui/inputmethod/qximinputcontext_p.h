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

#ifndef QXIMINPUTCONTEXT_P_H
#define QXIMINPUTCONTEXT_P_H

#include <qconfig.h>
#ifndef QT_NO_IM

#include "QtCore/qglobal.h"
#include "QtGui/qinputcontext.h"
#include "QtGui/qfont.h"
#include "QtCore/qhash.h"

#ifdef Q_WS_X11
#include "QtCore/qlist.h"
#include "QtCore/qbitarray.h"
#include "QtGui/qwindowdefs.h"
#include "qt_x11_p.h"
#endif

QT_BEGIN_NAMESPACE

class QKeyEvent;
class QWidget;
class QFont;
class QString;

class QXIMInputContext : public QInputContext
{
    CS_OBJECT(QXIMInputContext)

public:
    struct ICData {
        XIC ic;
        XFontSet fontset;
        QWidget *widget;
        QString text;
        QBitArray selectedChars;
        bool composing;
        bool preeditEmpty;
        void clear();
    };

    QXIMInputContext();
    ~QXIMInputContext();

    QString identifierName();
    QString language();

    void reset();

    void mouseHandler( int x, QMouseEvent *event);
    bool isComposing() const;

    void setFocusWidget( QWidget *w );
    void widgetDestroyed(QWidget *w);

    void create_xim();
    void close_xim();

    void update();

    ICData *icData() const;
protected:
    bool x11FilterEvent( QWidget *keywidget, XEvent *event );

private:
    static XIMStyle xim_style;

    QString _language;
    XIM xim;
    QHash<WId, ICData *> ximData;

    ICData *createICData(QWidget *w);
};

QT_END_NAMESPACE

#endif // Q_NO_IM

#endif // QXIMINPUTCONTEXT_P_H
