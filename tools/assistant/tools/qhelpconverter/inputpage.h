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

#ifndef INPUTPAGE_H
#define INPUTPAGE_H

#include <QtGui/QWizardPage>
#include "ui_inputpage.h"

QT_BEGIN_NAMESPACE

class AdpReader;

class InputPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit InputPage(AdpReader *reader, QWidget *parent = 0);
    
private slots:
    void getFileName();

private:
    bool validatePage();

    Ui::InputPage m_ui;
    AdpReader *m_adpReader;
};

QT_END_NAMESPACE

#endif
