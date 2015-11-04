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

#ifndef QCOCOAMENULOADER_MAC_P_H
#define QCOCOAMENULOADER_MAC_P_H

#include "qmacdefines_mac.h"
#ifdef QT_MAC_USE_COCOA
#import <Cocoa/Cocoa.h>

@interface QT_MANGLE_NAMESPACE(QCocoaMenuLoader) : NSResponder
{
    IBOutlet NSMenu *theMenu;
    IBOutlet NSMenu *appMenu;
    IBOutlet NSMenuItem *quitItem;
    IBOutlet NSMenuItem *preferencesItem;
    IBOutlet NSMenuItem *aboutItem;
    IBOutlet NSMenuItem *aboutQtItem;
    IBOutlet NSMenuItem *hideItem;
    NSMenuItem *lastAppSpecificItem;
    NSMenuItem *servicesItem;
    NSMenuItem *hideAllOthersItem;
    NSMenuItem *showAllItem;
}
- (void)ensureAppMenuInMenu:(NSMenu *)menu;
- (void)removeActionsFromAppMenu;
- (NSMenu *)applicationMenu;
- (NSMenu *)menu;
- (NSMenuItem *)quitMenuItem;
- (NSMenuItem *)preferencesMenuItem;
- (NSMenuItem *)aboutMenuItem;
- (NSMenuItem *)aboutQtMenuItem;
- (NSMenuItem *)hideMenuItem;
- (NSMenuItem *)appSpecificMenuItem:(NSInteger)tag;
- (IBAction)terminate:(id)sender;
- (IBAction)orderFrontStandardAboutPanel:(id)sender;
- (IBAction)hideOtherApplications:(id)sender;
- (IBAction)unhideAllApplications:(id)sender;
- (IBAction)hide:(id)sender;
- (IBAction)qtDispatcherToQAction:(id)sender;
- (void)qtUpdateMenubar;
- (void)orderFrontCharacterPalette:(id)sender;
- (BOOL)validateMenuItem:(NSMenuItem*)menuItem;
@end

#endif // QT_MAC_USE_COCOA
#endif // QCOCOAMENULOADER_P_H
