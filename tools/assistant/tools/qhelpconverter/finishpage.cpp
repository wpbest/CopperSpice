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

#include <QtGui/QTextEdit>
#include <QtGui/QLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QApplication>

#include "finishpage.h"

QT_BEGIN_NAMESPACE

FinishPage::FinishPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Converting File"));
    setSubTitle(tr("Creating the new Qt help files from the old ADP file."));
    setFinalPage(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum,
        QSizePolicy::Fixed));

    m_textEdit = new QTextEdit();
    layout->addWidget(m_textEdit);

    layout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum,
        QSizePolicy::Expanding));
}

void FinishPage::appendMessage(const QString &msg)
{
    m_textEdit->append(msg);
    qApp->processEvents();
}

QT_END_NAMESPACE
