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

#ifndef TEXTEDITFINDWIDGET_H
#define TEXTEDITFINDWIDGET_H

#include "abstractfindwidget.h"

QT_BEGIN_NAMESPACE

class QTextEdit;

class TextEditFindWidget : public AbstractFindWidget
{
    Q_OBJECT

public:
    explicit TextEditFindWidget(FindFlags flags = FindFlags(), QWidget *parent = 0);

    QTextEdit *textEdit() const
    { return m_textEdit; }

    void setTextEdit(QTextEdit *textEdit);

protected:
    virtual void deactivate();
    virtual void find(const QString &textToFind, bool skipCurrent, bool backward, bool *found, bool *wrapped);

private:
    QTextEdit *m_textEdit;
};

QT_END_NAMESPACE

#endif  // TEXTEDITFINDWIDGET_H
