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

#ifndef OUTPUTPAGE_H
#define OUTPUTPAGE_H

#include <QtGui/QWizardPage>
#include "ui_outputpage.h"

QT_BEGIN_NAMESPACE

class OutputPage : public QWizardPage
{
    Q_OBJECT

public:
    OutputPage(QWidget *parent = 0);
    void setPath(const QString &path);
    void setCollectionComponentEnabled(bool enabled);

private:
    bool isComplete() const;
    bool validatePage();
    bool checkFile(const QString &fileName,
        const QString &title);

    Ui::OutputPage m_ui;
    QString m_path;
};

QT_END_NAMESPACE

#endif
