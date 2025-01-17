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

#ifndef QPushButton_P_H
#define QPushButton_P_H

#include "qabstractbutton_p.h"

QT_BEGIN_NAMESPACE

class QDialog;
class QPushButton;

class QPushButtonPrivate : public QAbstractButtonPrivate
{
    Q_DECLARE_PUBLIC(QPushButton)

public:
    enum AutoDefaultValue { Off = 0, On = 1, Auto = 2 };

    QPushButtonPrivate()
        : QAbstractButtonPrivate(QSizePolicy::PushButton), autoDefault(Auto),
          defaultButton(false), flat(false), menuOpen(false), lastAutoDefault(false) {}

    inline void init() { resetLayoutItemMargins(); }
    static QPushButtonPrivate* get(QPushButton *b) { return b->d_func(); }

#ifdef Q_WS_MAC
    bool hitButton(const QPoint &pos);
#endif

#ifndef QT_NO_MENU
    QPoint adjustedMenuPosition();
#endif

    void resetLayoutItemMargins();
    void _q_popupPressed();
    QDialog *dialogParent() const;

    QPointer<QMenu> menu;
    uint autoDefault : 2;
    uint defaultButton : 1;
    uint flat : 1;
    uint menuOpen : 1;
    mutable uint lastAutoDefault : 1;
};

QT_END_NAMESPACE

#endif