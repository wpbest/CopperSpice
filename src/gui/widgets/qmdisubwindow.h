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

#ifndef QMDISUBWINDOW_H
#define QMDISUBWINDOW_H

#include <QtGui/qwidget.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_MDIAREA

class QMenu;
class QMdiArea;

namespace QMdi 
   { class ControlContainer; }

class QMdiSubWindowPrivate;

class Q_GUI_EXPORT QMdiSubWindow : public QWidget
{
    CS_OBJECT(QMdiSubWindow)

    GUI_CS_PROPERTY_READ(keyboardSingleStep, keyboardSingleStep)
    GUI_CS_PROPERTY_WRITE(keyboardSingleStep, setKeyboardSingleStep)
    GUI_CS_PROPERTY_READ(keyboardPageStep, keyboardPageStep)
    GUI_CS_PROPERTY_WRITE(keyboardPageStep, setKeyboardPageStep)

public:
    enum SubWindowOption {
        AllowOutsideAreaHorizontally = 0x1, // internal
        AllowOutsideAreaVertically = 0x2, // internal
        RubberBandResize = 0x4,
        RubberBandMove = 0x8
    };
    using SubWindowOptions = QFlags<SubWindowOption>;

    QMdiSubWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~QMdiSubWindow();

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    void setWidget(QWidget *widget);
    QWidget *widget() const;

    QWidget *maximizedButtonsWidget() const; // internal
    QWidget *maximizedSystemMenuIconWidget() const; // internal

    bool isShaded() const;

    void setOption(SubWindowOption option, bool on = true);
    bool testOption(SubWindowOption) const;

    void setKeyboardSingleStep(int step);
    int keyboardSingleStep() const;

    void setKeyboardPageStep(int step);
    int keyboardPageStep() const;

#ifndef QT_NO_MENU
    void setSystemMenu(QMenu *systemMenu);
    QMenu *systemMenu() const;
#endif

    QMdiArea *mdiArea() const;

    GUI_CS_SIGNAL_1(Public, void windowStateChanged(Qt::WindowStates oldState,Qt::WindowStates newState))
    GUI_CS_SIGNAL_2(windowStateChanged,oldState,newState) 
    GUI_CS_SIGNAL_1(Public, void aboutToActivate())
    GUI_CS_SIGNAL_2(aboutToActivate) 

#ifndef QT_NO_MENU
    GUI_CS_SLOT_1(Public, void showSystemMenu())
    GUI_CS_SLOT_2(showSystemMenu) 
#endif
    GUI_CS_SLOT_1(Public, void showShaded())
    GUI_CS_SLOT_2(showShaded) 

protected:
    bool eventFilter(QObject *object, QEvent *event);
    bool event(QEvent *event);
    void showEvent(QShowEvent *showEvent);
    void hideEvent(QHideEvent *hideEvent);
    void changeEvent(QEvent *changeEvent);
    void closeEvent(QCloseEvent *closeEvent);
    void leaveEvent(QEvent *leaveEvent);
    void resizeEvent(QResizeEvent *resizeEvent);
    void timerEvent(QTimerEvent *timerEvent);
    void moveEvent(QMoveEvent *moveEvent);
    void paintEvent(QPaintEvent *paintEvent);
    void mousePressEvent(QMouseEvent *mouseEvent);
    void mouseDoubleClickEvent(QMouseEvent *mouseEvent);
    void mouseReleaseEvent(QMouseEvent *mouseEvent);
    void mouseMoveEvent(QMouseEvent *mouseEvent);
    void keyPressEvent(QKeyEvent *keyEvent);

#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *contextMenuEvent);
#endif

    void focusInEvent(QFocusEvent *focusInEvent);
    void focusOutEvent(QFocusEvent *focusOutEvent);
    void childEvent(QChildEvent *childEvent);

private:
    Q_DISABLE_COPY(QMdiSubWindow)
    Q_DECLARE_PRIVATE(QMdiSubWindow)

    GUI_CS_SLOT_1(Private, void _q_updateStaysOnTopHint())
    GUI_CS_SLOT_2(_q_updateStaysOnTopHint)

    GUI_CS_SLOT_1(Private, void _q_enterInteractiveMode())
    GUI_CS_SLOT_2(_q_enterInteractiveMode)

    GUI_CS_SLOT_1(Private, void _q_processFocusChanged(QWidget * un_named_arg1,QWidget * un_named_arg2))
    GUI_CS_SLOT_2(_q_processFocusChanged)

    friend class QMdiAreaPrivate;
    friend class QMdi::ControlContainer;

#ifndef QT_NO_TABBAR
    friend class QMdiAreaTabBar;
#endif
    
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QMdiSubWindow::SubWindowOptions)

QT_END_NAMESPACE

#endif // QT_NO_MDIAREA

#endif // QMDISUBWINDOW_H
