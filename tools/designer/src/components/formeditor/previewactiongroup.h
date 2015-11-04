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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of Qt Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef PREVIEWACTIONGROUP_H
#define PREVIEWACTIONGROUP_H

#include <QtGui/QActionGroup>

QT_BEGIN_NAMESPACE

class QDesignerFormEditorInterface;

namespace qdesigner_internal {

/* PreviewActionGroup: To be used as a submenu for 'Preview in...'
 * Offers a menu of styles and device profiles. */

class PreviewActionGroup : public QActionGroup
{
    Q_DISABLE_COPY(PreviewActionGroup)
    Q_OBJECT
public:
    explicit PreviewActionGroup(QDesignerFormEditorInterface *core, QObject *parent = 0);

signals:
    void preview(const QString &style, int deviceProfileIndex);

public slots:
    void updateDeviceProfiles();

private  slots:
    void slotTriggered(QAction *);

private:
    QDesignerFormEditorInterface *m_core;
};
}

QT_END_NAMESPACE

#endif // PREVIEWACTIONGROUP_H
