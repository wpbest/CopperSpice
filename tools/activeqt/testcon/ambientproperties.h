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

#ifndef AMBIENTPROPERTIES_H
#define AMBIENTPROPERTIES_H

#include <QtCore/qglobal.h>

#include "ui_ambientproperties.h"

QT_BEGIN_NAMESPACE

class AmbientProperties : public QDialog, Ui::AmbientProperties
{
    Q_OBJECT
public:
    AmbientProperties(QWidget *parent);

    void setControl(QWidget *widget);

public slots:
    void on_buttonBackground_clicked();
    void on_buttonForeground_clicked();
    void on_buttonFont_clicked();
    void on_buttonEnabled_toggled(bool on);

private:
    QWidget *container;
};

QT_END_NAMESPACE

#endif // AMBIENTPROPERTIES_H
