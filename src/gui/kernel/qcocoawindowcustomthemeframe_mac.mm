/***********************************************************************
*
* Copyright (c) 2012-2014 Barbara Geller
* Copyright (c) 2012-2014 Ansel Sermersheim
* Copyright (c) 2012-2014 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
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

#include "qmacdefines_mac.h"

#ifdef QT_MAC_USE_COCOA

#import "private/qcocoawindowcustomthemeframe_mac_p.h"
#import "private/qcocoawindow_mac_p.h"
#include "private/qt_cocoa_helpers_mac_p.h"
#include "qwidget.h"

@implementation QT_MANGLE_NAMESPACE(QCocoaWindowCustomThemeFrame)

- (void)_updateButtons
{
    [super _updateButtons];
    NSWindow *window = [self window];
    qt_syncCocoaTitleBarButtons(window, [window QT_MANGLE_NAMESPACE(qt_qwidget)]);
}

@end

#endif
