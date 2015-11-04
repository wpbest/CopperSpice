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

#ifndef OPENPAGESMODEL_H
#define OPENPAGESMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QList>

QT_BEGIN_NAMESPACE

class HelpViewer;
class QUrl;

class OpenPagesModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    OpenPagesModel(QObject *parent);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void addPage(const QUrl &url, qreal zoom = 0);
    void removePage(int index);
    HelpViewer *pageAt(int index) const;

private slots:
    void handleTitleChanged();

private:
    QList<HelpViewer *> m_pages;
};

QT_END_NAMESPACE

#endif // OPENPAGESMODEL_H
