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

#ifndef QVFBRATEDLG_H
#define QVFBRATEDLG_H

#include <QDialog>

QT_BEGIN_NAMESPACE

class QLabel;
class QSlider;

class QVFbRateDialog : public QDialog
{
    Q_OBJECT
public:
    QVFbRateDialog(int value, QWidget *parent = 0);

signals:
    void updateRate(int r);

protected slots:
    void rateChanged(int r);
    void cancel();
    void ok();

private:
    QLabel *rateLabel;
    QSlider *rateSlider;
    int oldRate;
};

QT_END_NAMESPACE

#endif
