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

#ifndef LABEL_TASKMENU_H
#define LABEL_TASKMENU_H

#include <QtGui/QLabel>
#include <QtCore/QPointer>

#include <qdesigner_taskmenu_p.h>
#include <extensionfactory_p.h>

QT_BEGIN_NAMESPACE

class QDesignerFormWindowInterface;

namespace qdesigner_internal {

class LabelTaskMenu: public QDesignerTaskMenu
{
    Q_OBJECT
public:
    explicit LabelTaskMenu(QLabel *button, QObject *parent = 0);

    virtual QAction *preferredEditAction() const;
    virtual QList<QAction*> taskActions() const;

private slots:
    void editRichText();

private:
    QLabel *m_label;
    QList<QAction*> m_taskActions;
    QAction *m_editRichTextAction;
    QAction *m_editPlainTextAction;
};

typedef ExtensionFactory<QDesignerTaskMenuExtension, QLabel, LabelTaskMenu>  LabelTaskMenuFactory;
}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // LABEL_TASKMENU_H
