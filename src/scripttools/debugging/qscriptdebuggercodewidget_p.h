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

#ifndef QSCRIPTDEBUGGERCODEWIDGET_P_H
#define QSCRIPTDEBUGGERCODEWIDGET_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qscriptdebuggercodewidgetinterface_p.h"

QT_BEGIN_NAMESPACE

class QScriptDebuggerCodeWidgetPrivate;
class QScriptDebuggerCodeWidget:
    public QScriptDebuggerCodeWidgetInterface
{
    CS_OBJECT(QScriptDebuggerCodeWidget)
public:
    QScriptDebuggerCodeWidget(QWidget *parent = 0);
    ~QScriptDebuggerCodeWidget();

    QScriptDebuggerScriptsModel *scriptsModel() const;
    void setScriptsModel(QScriptDebuggerScriptsModel *model);

    void setToolTipProvider(QScriptToolTipProviderInterface *toolTipProvider);

    qint64 currentScriptId() const;
    void setCurrentScript(qint64 scriptId);

    void invalidateExecutionLineNumbers();

    QScriptBreakpointsModel *breakpointsModel() const;
    void setBreakpointsModel(QScriptBreakpointsModel *model);

    QScriptDebuggerCodeViewInterface *currentView() const;

private:
    Q_DECLARE_PRIVATE(QScriptDebuggerCodeWidget)
    Q_DISABLE_COPY(QScriptDebuggerCodeWidget)

    CS_SLOT_1(Private, void _q_onBreakpointToggleRequest(int un_named_arg1,bool un_named_arg2))
    CS_SLOT_2(_q_onBreakpointToggleRequest)

/*  PRIVATE_SLOT
void _q_onBreakpointToggleRequest(int un_named_arg1,bool un_named_arg2)
{
	Q_D(QScriptDebuggerCodeWidget);
	d->_q_onBreakpointToggleRequest();
}
*/
    CS_SLOT_1(Private, void _q_onBreakpointEnableRequest(int un_named_arg1,bool un_named_arg2))
    CS_SLOT_2(_q_onBreakpointEnableRequest)

/*  PRIVATE_SLOT
void _q_onBreakpointEnableRequest(int un_named_arg1,bool un_named_arg2)
{
	Q_D(QScriptDebuggerCodeWidget);
	d->_q_onBreakpointEnableRequest();
}
*/
    CS_SLOT_1(Private, void _q_onBreakpointsAboutToBeRemoved(const QModelIndex & un_named_arg1,int un_named_arg2,int un_named_arg3))
    CS_SLOT_2(_q_onBreakpointsAboutToBeRemoved)

/*  PRIVATE_SLOT
void _q_onBreakpointsAboutToBeRemoved(const QModelIndex & un_named_arg1,int un_named_arg2,int un_named_arg3)
{
	Q_D(QScriptDebuggerCodeWidget);
	d->_q_onBreakpointsAboutToBeRemoved();
}
*/
    CS_SLOT_1(Private, void _q_onBreakpointsInserted(const QModelIndex & un_named_arg1,int un_named_arg2,int un_named_arg3))
    CS_SLOT_2(_q_onBreakpointsInserted)

/*  PRIVATE_SLOT
void _q_onBreakpointsInserted(const QModelIndex & un_named_arg1,int un_named_arg2,int un_named_arg3)
{
	Q_D(QScriptDebuggerCodeWidget);
	d->_q_onBreakpointsInserted();
}
*/
    CS_SLOT_1(Private, void _q_onBreakpointsDataChanged(const QModelIndex & un_named_arg1,const QModelIndex & un_named_arg2))
    CS_SLOT_2(_q_onBreakpointsDataChanged)

/*  PRIVATE_SLOT
void _q_onBreakpointsDataChanged(const QModelIndex & un_named_arg1,const QModelIndex & un_named_arg2)
{
	Q_D(QScriptDebuggerCodeWidget);
	d->_q_onBreakpointsDataChanged();
}
*/
    CS_SLOT_1(Private, void _q_onScriptsChanged())
    CS_SLOT_2(_q_onScriptsChanged)

/*  PRIVATE_SLOT
void _q_onScriptsChanged()
{
	Q_D(QScriptDebuggerCodeWidget);
	d->_q_onScriptsChanged();
}
*/
    CS_SLOT_1(Private, void _q_onToolTipRequest(const QPoint & un_named_arg1,int un_named_arg2,const QStringList & un_named_arg3))
    CS_SLOT_2(_q_onToolTipRequest)

/*  PRIVATE_SLOT
void _q_onToolTipRequest(const QPoint & un_named_arg1,int un_named_arg2,const QStringList & un_named_arg3)
{
	Q_D(QScriptDebuggerCodeWidget);
	d->_q_onToolTipRequest();
}
*/
};

QT_END_NAMESPACE

#endif
