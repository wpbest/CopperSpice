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

#ifndef QTREEVIEW_H
#define QTREEVIEW_H

#include <QtGui/qabstractitemview.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_TREEVIEW

class QTreeViewPrivate;
class QHeaderView;

class Q_GUI_EXPORT QTreeView : public QAbstractItemView
{
    CS_OBJECT(QTreeView)

    GUI_CS_PROPERTY_READ(autoExpandDelay, autoExpandDelay)
    GUI_CS_PROPERTY_WRITE(autoExpandDelay, setAutoExpandDelay)
    GUI_CS_PROPERTY_READ(indentation, indentation)
    GUI_CS_PROPERTY_WRITE(indentation, setIndentation)
    GUI_CS_PROPERTY_READ(rootIsDecorated, rootIsDecorated)
    GUI_CS_PROPERTY_WRITE(rootIsDecorated, setRootIsDecorated)
    GUI_CS_PROPERTY_READ(uniformRowHeights, uniformRowHeights)
    GUI_CS_PROPERTY_WRITE(uniformRowHeights, setUniformRowHeights)
    GUI_CS_PROPERTY_READ(itemsExpandable, itemsExpandable)
    GUI_CS_PROPERTY_WRITE(itemsExpandable, setItemsExpandable)
    GUI_CS_PROPERTY_READ(sortingEnabled, isSortingEnabled)
    GUI_CS_PROPERTY_WRITE(sortingEnabled, setSortingEnabled)
    GUI_CS_PROPERTY_READ(animated, isAnimated)
    GUI_CS_PROPERTY_WRITE(animated, setAnimated)
    GUI_CS_PROPERTY_READ(allColumnsShowFocus, allColumnsShowFocus)
    GUI_CS_PROPERTY_WRITE(allColumnsShowFocus, setAllColumnsShowFocus)
    GUI_CS_PROPERTY_READ(wordWrap, wordWrap)
    GUI_CS_PROPERTY_WRITE(wordWrap, setWordWrap)
    GUI_CS_PROPERTY_READ(headerHidden, isHeaderHidden)
    GUI_CS_PROPERTY_WRITE(headerHidden, setHeaderHidden)
    GUI_CS_PROPERTY_READ(expandsOnDoubleClick, expandsOnDoubleClick)
    GUI_CS_PROPERTY_WRITE(expandsOnDoubleClick, setExpandsOnDoubleClick)

public:
    explicit QTreeView(QWidget *parent = 0);
    ~QTreeView();

    void setModel(QAbstractItemModel *model);
    void setRootIndex(const QModelIndex &index);
    void setSelectionModel(QItemSelectionModel *selectionModel);

    QHeaderView *header() const;
    void setHeader(QHeaderView *header);

    int autoExpandDelay() const;
    void setAutoExpandDelay(int delay);

    int indentation() const;
    void setIndentation(int i);

    bool rootIsDecorated() const;
    void setRootIsDecorated(bool show);

    bool uniformRowHeights() const;
    void setUniformRowHeights(bool uniform);

    bool itemsExpandable() const;
    void setItemsExpandable(bool enable);

    bool expandsOnDoubleClick() const;
    void setExpandsOnDoubleClick(bool enable);

    int columnViewportPosition(int column) const;
    int columnWidth(int column) const;
    void setColumnWidth(int column, int width);
    int columnAt(int x) const;

    bool isColumnHidden(int column) const;
    void setColumnHidden(int column, bool hide);

    bool isHeaderHidden() const;
    void setHeaderHidden(bool hide);

    bool isRowHidden(int row, const QModelIndex &parent) const;
    void setRowHidden(int row, const QModelIndex &parent, bool hide);

    bool isFirstColumnSpanned(int row, const QModelIndex &parent) const;
    void setFirstColumnSpanned(int row, const QModelIndex &parent, bool span);

    bool isExpanded(const QModelIndex &index) const;
    void setExpanded(const QModelIndex &index, bool expand);

    void setSortingEnabled(bool enable);
    bool isSortingEnabled() const;

    void setAnimated(bool enable);
    bool isAnimated() const;

    void setAllColumnsShowFocus(bool enable);
    bool allColumnsShowFocus() const;

    void setWordWrap(bool on);
    bool wordWrap() const;

    void keyboardSearch(const QString &search);

    QRect visualRect(const QModelIndex &index) const;
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
    QModelIndex indexAt(const QPoint &p) const;
    QModelIndex indexAbove(const QModelIndex &index) const;
    QModelIndex indexBelow(const QModelIndex &index) const;

    void doItemsLayout();
    void reset();

    void sortByColumn(int column, Qt::SortOrder order);

    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void selectAll();

    GUI_CS_SIGNAL_1(Public, void expanded(const QModelIndex & index))
    GUI_CS_SIGNAL_2(expanded,index) 
  
    GUI_CS_SIGNAL_1(Public, void collapsed(const QModelIndex & index))
    GUI_CS_SIGNAL_2(collapsed,index) 

    GUI_CS_SLOT_1(Public, void hideColumn(int column))
    GUI_CS_SLOT_2(hideColumn) 

    GUI_CS_SLOT_1(Public, void showColumn(int column))
    GUI_CS_SLOT_2(showColumn) 

    GUI_CS_SLOT_1(Public, void expand(const QModelIndex & index))
    GUI_CS_SLOT_2(expand) 

    GUI_CS_SLOT_1(Public, void collapse(const QModelIndex & index))
    GUI_CS_SLOT_2(collapse) 

    GUI_CS_SLOT_1(Public, void resizeColumnToContents(int column))
    GUI_CS_SLOT_2(resizeColumnToContents) 

    GUI_CS_SLOT_1(Public, void sortByColumn(int column))
    GUI_CS_SLOT_OVERLOAD(sortByColumn,(int))

    GUI_CS_SLOT_1(Public, void expandAll())
    GUI_CS_SLOT_2(expandAll) 

    GUI_CS_SLOT_1(Public, void collapseAll())
    GUI_CS_SLOT_2(collapseAll) 

    GUI_CS_SLOT_1(Public, void expandToDepth(int depth))
    GUI_CS_SLOT_2(expandToDepth) 

protected :
    GUI_CS_SLOT_1(Protected, void columnResized(int column,int oldSize,int newSize))
    GUI_CS_SLOT_2(columnResized) 
    GUI_CS_SLOT_1(Protected, void columnCountChanged(int oldCount,int newCount))
    GUI_CS_SLOT_2(columnCountChanged) 
    GUI_CS_SLOT_1(Protected, void columnMoved())
    GUI_CS_SLOT_2(columnMoved) 
    GUI_CS_SLOT_1(Protected, void reexpand())
    GUI_CS_SLOT_2(reexpand) 
    GUI_CS_SLOT_1(Protected, void rowsRemoved(const QModelIndex & parent,int first,int last))
    GUI_CS_SLOT_2(rowsRemoved) 

    QTreeView(QTreeViewPrivate &dd, QWidget *parent = 0);
    void scrollContentsBy(int dx, int dy);
    void rowsInserted(const QModelIndex &parent, int start, int end);
    void rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);

    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
    int horizontalOffset() const;
    int verticalOffset() const;

    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command);
    QRegion visualRegionForSelection(const QItemSelection &selection) const;
    QModelIndexList selectedIndexes() const;

    void timerEvent(QTimerEvent *event);
    void paintEvent(QPaintEvent *event);

    void drawTree(QPainter *painter, const QRegion &region) const;
    virtual void drawRow(QPainter *painter, const QStyleOptionViewItem &options,const QModelIndex &index) const;
    virtual void drawBranches(QPainter *painter,const QRect &rect,const QModelIndex &index) const;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

#ifndef QT_NO_DRAGANDDROP
    void dragMoveEvent(QDragMoveEvent *event);
#endif

    bool viewportEvent(QEvent *event);

    void updateGeometries();

    int sizeHintForColumn(int column) const;
    int indexRowSizeHint(const QModelIndex &index) const;
    int rowHeight(const QModelIndex &index) const;

    void horizontalScrollbarAction(int action);

    bool isIndexHidden(const QModelIndex &index) const;
    void selectionChanged(const QItemSelection &selected,const QItemSelection &deselected);
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    friend class QAccessibleItemView;
    friend class QAccessibleTable2;
    friend class QAccessibleTree;
    friend class QAccessibleTable2Cell;
    int visualIndex(const QModelIndex &index) const;

    Q_DECLARE_PRIVATE(QTreeView)
    Q_DISABLE_COPY(QTreeView)

#ifndef QT_NO_ANIMATION
    GUI_CS_SLOT_1(Private, void _q_endAnimatedOperation())
    GUI_CS_SLOT_2(_q_endAnimatedOperation)
#endif

    GUI_CS_SLOT_1(Private, void _q_modelAboutToBeReset())
    GUI_CS_SLOT_2(_q_modelAboutToBeReset)

    GUI_CS_SLOT_1(Private, void _q_sortIndicatorChanged(int column,Qt::SortOrder order))
    GUI_CS_SLOT_2(_q_sortIndicatorChanged)

};

#endif // QT_NO_TREEVIEW

QT_END_NAMESPACE

#endif // QTREEVIEW_H
