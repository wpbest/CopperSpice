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

#include "stringlisteditorbutton.h"
#include "stringlisteditor.h"

#include <QtCore/qdebug.h>

QT_BEGIN_NAMESPACE

using namespace qdesigner_internal;

StringListEditorButton::StringListEditorButton(
    const QStringList &stringList, QWidget *parent)
    : QToolButton(parent), m_stringList(stringList)
{
    setFocusPolicy(Qt::NoFocus);
    setText(tr("Change String List"));
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));

    connect(this, SIGNAL(clicked()), this, SLOT(showStringListEditor()));
}

StringListEditorButton::~StringListEditorButton()
{
}

void StringListEditorButton::setStringList(const QStringList &stringList)
{
    m_stringList = stringList;
}

void StringListEditorButton::showStringListEditor()
{
    int result;
    QStringList lst = StringListEditor::getStringList(0, m_stringList, &result);
    if (result == QDialog::Accepted) {
        m_stringList = lst;
        emit stringListChanged(m_stringList);
    }
}

QT_END_NAMESPACE
