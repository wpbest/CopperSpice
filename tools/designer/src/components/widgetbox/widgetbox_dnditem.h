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

#ifndef WIDGETBOX_DNDITEM_H
#define WIDGETBOX_DNDITEM_H

#include <qdesigner_dnditem_p.h>
#include "widgetbox_global.h"

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;
class DomUI;

namespace qdesigner_internal {

class QT_WIDGETBOX_EXPORT WidgetBoxDnDItem : public QDesignerDnDItem
{
public:
    WidgetBoxDnDItem(QDesignerFormEditorInterface *core,
                     DomUI *dom_ui,
                     const QPoint &global_mouse_pos);
};

}  // namespace qdesigner_internal

QT_END_NAMESPACE

#endif // WIDGETBOX_DNDITEM_H
