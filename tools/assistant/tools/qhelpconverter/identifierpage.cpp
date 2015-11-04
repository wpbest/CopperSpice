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

#include "identifierpage.h"

QT_BEGIN_NAMESPACE

IdentifierPage::IdentifierPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Identifiers"));
    setSubTitle(tr("This page allows you to create identifiers from "
        "the keywords found in the .adp or .dcf file."));

    m_ui.setupUi(this);
    
    connect(m_ui.identifierCheckBox, SIGNAL(toggled(bool)),
        this, SLOT(setupButtons(bool)));

    registerField(QLatin1String("createIdentifier"), m_ui.identifierCheckBox);
    registerField(QLatin1String("globalPrefix"), m_ui.prefixLineEdit);
    registerField(QLatin1String("fileNamePrefix"), m_ui.fileNameButton);
}

void IdentifierPage::setupButtons(bool checked)
{
    m_ui.globalButton->setEnabled(checked);
    m_ui.fileNameButton->setEnabled(checked);
    m_ui.prefixLineEdit->setEnabled(checked 
        && m_ui.globalButton->isChecked());
}

QT_END_NAMESPACE
