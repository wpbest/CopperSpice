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

#ifndef LISTWIDGETEDITOR_H
#define LISTWIDGETEDITOR_H

#include "itemlisteditor.h"
#include <qdesigner_command_p.h>

#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class QListWidget;
class QComboBox;
class QDesignerFormWindowInterface;

namespace qdesigner_internal {

class ListWidgetEditor: public QDialog
{
    Q_OBJECT

public:
    ListWidgetEditor(QDesignerFormWindowInterface *form,
                     QWidget *parent);

    ListContents fillContentsFromListWidget(QListWidget *listWidget);
    ListContents fillContentsFromComboBox(QComboBox *comboBox);
    ListContents contents() const;

private:
    ItemListEditor *m_itemsEditor;
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // LISTWIDGETEDITOR_H
