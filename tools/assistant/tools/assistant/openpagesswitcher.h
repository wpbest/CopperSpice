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

#ifndef OPENPAGESSWITCHER_H
#define OPENPAGESSWITCHER_H

#include <QtGui/QFrame>

QT_BEGIN_NAMESPACE

class OpenPagesModel;
class OpenPagesWidget;
class QModelIndex;

class OpenPagesSwitcher : public QFrame
{
    Q_OBJECT

public:
    OpenPagesSwitcher(OpenPagesModel *model);
    ~OpenPagesSwitcher();

    void gotoNextPage();
    void gotoPreviousPage();

    void selectAndHide();
    void selectCurrentPage();

    void setVisible(bool visible);
    void focusInEvent(QFocusEvent *event);
    bool eventFilter(QObject *object, QEvent *event);

signals:
    void closePage(const QModelIndex &index);
    void setCurrentPage(const QModelIndex &index);

private:
    void selectPageUpDown(int summand);

private:
    OpenPagesModel *m_openPagesModel;
    OpenPagesWidget *m_openPagesWidget;
};

QT_END_NAMESPACE

#endif  // OPENPAGESSWITCHER_H
