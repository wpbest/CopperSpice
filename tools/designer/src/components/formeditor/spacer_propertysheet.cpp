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

#include "spacer_propertysheet.h"
#include "qdesigner_widget_p.h"
#include "formwindow.h"
#include "spacer_widget_p.h"

#include <QtDesigner/QExtensionManager>

#include <QtGui/QLayout>

QT_BEGIN_NAMESPACE

namespace qdesigner_internal
{
SpacerPropertySheet::SpacerPropertySheet(Spacer *object, QObject *parent)
    : QDesignerPropertySheet(object, parent)
{
    clearFakeProperties();
}

SpacerPropertySheet::~SpacerPropertySheet()
{
}

bool SpacerPropertySheet::isVisible(int index) const
{
    static const QString spacerGroup = QLatin1String("Spacer");
    return propertyGroup(index) == spacerGroup;
}

void SpacerPropertySheet::setProperty(int index, const QVariant &value)
{
    QDesignerPropertySheet::setProperty(index, value);
}

bool SpacerPropertySheet::dynamicPropertiesAllowed() const
{
    return false;
}
}

QT_END_NAMESPACE
