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

#ifndef QGRIDLAYOUT_H
#define QGRIDLAYOUT_H

#include <QtGui/qlayout.h>
#include <limits.h>

QT_BEGIN_NAMESPACE

class QGridLayoutPrivate;

class Q_GUI_EXPORT QGridLayout : public QLayout
{
    CS_OBJECT(QGridLayout)
    Q_DECLARE_PRIVATE(QGridLayout)

    // following 4 were qdoc_property 1/5/2014
    GUI_CS_PROPERTY_READ(horizontalSpacing, horizontalSpacing)
    GUI_CS_PROPERTY_WRITE(horizontalSpacing, setHorizontalSpacing)
    GUI_CS_PROPERTY_READ(verticalSpacing, verticalSpacing)
    GUI_CS_PROPERTY_WRITE(verticalSpacing, setVerticalSpacing)

public:
    explicit QGridLayout(QWidget *parent);
    QGridLayout();
    ~QGridLayout();

    QSize sizeHint() const;
    QSize minimumSize() const;
    QSize maximumSize() const;

    void setHorizontalSpacing(int spacing);
    int horizontalSpacing() const;
    void setVerticalSpacing(int spacing);
    int verticalSpacing() const;
    void setSpacing(int spacing);
    int spacing() const;

    void setRowStretch(int row, int stretch);
    void setColumnStretch(int column, int stretch);
    int rowStretch(int row) const;
    int columnStretch(int column) const;

    void setRowMinimumHeight(int row, int minSize);
    void setColumnMinimumWidth(int column, int minSize);
    int rowMinimumHeight(int row) const;
    int columnMinimumWidth(int column) const;

    int columnCount() const;
    int rowCount() const;

    QRect cellRect(int row, int column) const;

    bool hasHeightForWidth() const;
    int heightForWidth(int) const;
    int minimumHeightForWidth(int) const;

    Qt::Orientations expandingDirections() const;
    void invalidate();

    inline void addWidget(QWidget *w) { QLayout::addWidget(w); }
    void addWidget(QWidget *, int row, int column, Qt::Alignment = 0);
    void addWidget(QWidget *, int row, int column, int rowSpan, int columnSpan, Qt::Alignment = 0);
    void addLayout(QLayout *, int row, int column, Qt::Alignment = 0);
    void addLayout(QLayout *, int row, int column, int rowSpan, int columnSpan, Qt::Alignment = 0);

    void setOriginCorner(Qt::Corner);
    Qt::Corner originCorner() const;

    QLayoutItem *itemAt(int index) const;
    QLayoutItem *itemAtPosition(int row, int column) const;
    QLayoutItem *takeAt(int index);
    int count() const;
    void setGeometry(const QRect&);

    void addItem(QLayoutItem *item, int row, int column, int rowSpan = 1, int columnSpan = 1, Qt::Alignment = 0);

    void setDefaultPositioning(int n, Qt::Orientation orient);
    void getItemPosition(int idx, int *row, int *column, int *rowSpan, int *columnSpan);

protected:
    void addItem(QLayoutItem *);

private:
    Q_DISABLE_COPY(QGridLayout)
};

QT_END_NAMESPACE

#endif // QGRIDLAYOUT_H
