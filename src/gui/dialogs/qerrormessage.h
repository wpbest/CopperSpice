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

#ifndef QERRORMESSAGE_H
#define QERRORMESSAGE_H

#include <QtGui/qdialog.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_ERRORMESSAGE

class QErrorMessagePrivate;

class Q_GUI_EXPORT QErrorMessage: public QDialog
{
    CS_OBJECT(QErrorMessage)
    Q_DECLARE_PRIVATE(QErrorMessage)

public:
    explicit QErrorMessage(QWidget* parent = 0);
    ~QErrorMessage();

    static QErrorMessage * qtHandler();

    GUI_CS_SLOT_1(Public, void showMessage(const QString & message))
    GUI_CS_SLOT_OVERLOAD(showMessage,(const QString &)) 

    GUI_CS_SLOT_1(Public, void showMessage(const QString & message,const QString & type))
    GUI_CS_SLOT_OVERLOAD(showMessage,(const QString &, const QString &)) 

protected:
    void done(int);
    void changeEvent(QEvent *e);

private:
    Q_DISABLE_COPY(QErrorMessage)
};

#endif // QT_NO_ERRORMESSAGE

QT_END_NAMESPACE

#endif // QERRORMESSAGE_H
