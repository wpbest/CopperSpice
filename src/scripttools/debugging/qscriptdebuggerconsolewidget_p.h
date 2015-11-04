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

#ifndef QSCRIPTDEBUGGERCONSOLEWIDGET_P_H
#define QSCRIPTDEBUGGERCONSOLEWIDGET_P_H

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

#include "qscriptdebuggerconsolewidgetinterface_p.h"

QT_BEGIN_NAMESPACE

class QScriptDebuggerConsoleWidgetPrivate;
class QScriptDebuggerConsoleWidget:
    public QScriptDebuggerConsoleWidgetInterface
{
    CS_OBJECT(QScriptDebuggerConsoleWidget)
public:
    QScriptDebuggerConsoleWidget(QWidget *parent = 0);
    ~QScriptDebuggerConsoleWidget();

    void message(QtMsgType type, const QString &text,
                 const QString &fileName = QString(),
                 int lineNumber = -1, int columnNumber = -1,
                 const QVariant &data = QVariant());

    void setLineContinuationMode(bool enabled);

    void clear();

protected:
    void keyPressEvent(QKeyEvent *event);
    bool focusNextPrevChild(bool);

protected:
    QScriptDebuggerConsoleWidget(
        QScriptDebuggerConsoleWidgetPrivate &dd,
        QWidget *parent);

private:
    Q_DECLARE_PRIVATE(QScriptDebuggerConsoleWidget)
    Q_DISABLE_COPY(QScriptDebuggerConsoleWidget)

    CS_SLOT_1(Private, void _q_onLineEntered(const QString & un_named_arg1))
    CS_SLOT_2(_q_onLineEntered)

/*  PRIVATE_SLOT
void _q_onLineEntered(const QString & un_named_arg1)
{
	Q_D(QScriptDebuggerConsoleWidget);
	d->_q_onLineEntered();
}
*/
    CS_SLOT_1(Private, void _q_onLineEdited(const QString & un_named_arg1))
    CS_SLOT_2(_q_onLineEdited)

/*  PRIVATE_SLOT
void _q_onLineEdited(const QString & un_named_arg1)
{
	Q_D(QScriptDebuggerConsoleWidget);
	d->_q_onLineEdited();
}
*/
    CS_SLOT_1(Private, void _q_onCompletionTaskFinished())
    CS_SLOT_2(_q_onCompletionTaskFinished)

/*  PRIVATE_SLOT
void _q_onCompletionTaskFinished()
{
	Q_D(QScriptDebuggerConsoleWidget);
	d->_q_onCompletionTaskFinished();
}
*/
};

QT_END_NAMESPACE

#endif
