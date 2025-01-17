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

#ifndef QTOOLBOX_H
#define QTOOLBOX_H

#include <QtGui/qframe.h>
#include <QtGui/qicon.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_TOOLBOX

class QToolBoxPrivate;

class Q_GUI_EXPORT QToolBox : public QFrame
{
    CS_OBJECT(QToolBox)

    GUI_CS_PROPERTY_READ(currentIndex, currentIndex)
    GUI_CS_PROPERTY_WRITE(currentIndex, setCurrentIndex)
    GUI_CS_PROPERTY_NOTIFY(currentIndex, currentChanged)
    GUI_CS_PROPERTY_READ(count, count)

public:
    explicit QToolBox(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~QToolBox();

    int addItem(QWidget *widget, const QString &text);
    int addItem(QWidget *widget, const QIcon &icon, const QString &text);
    int insertItem(int index, QWidget *widget, const QString &text);
    int insertItem(int index, QWidget *widget, const QIcon &icon, const QString &text);

    void removeItem(int index);

    void setItemEnabled(int index, bool enabled);
    bool isItemEnabled(int index) const;

    void setItemText(int index, const QString &text);
    QString itemText(int index) const;

    void setItemIcon(int index, const QIcon &icon);
    QIcon itemIcon(int index) const;

#ifndef QT_NO_TOOLTIP
    void setItemToolTip(int index, const QString &toolTip);
    QString itemToolTip(int index) const;
#endif

    int currentIndex() const;
    QWidget *currentWidget() const;
    QWidget *widget(int index) const;
    int indexOf(QWidget *widget) const;
    int count() const;

    GUI_CS_SLOT_1(Public, void setCurrentIndex(int index))
    GUI_CS_SLOT_2(setCurrentIndex) 
    GUI_CS_SLOT_1(Public, void setCurrentWidget(QWidget * widget))
    GUI_CS_SLOT_2(setCurrentWidget) 

    GUI_CS_SIGNAL_1(Public, void currentChanged(int index))
    GUI_CS_SIGNAL_2(currentChanged,index) 

protected:
    bool event(QEvent *e);
    virtual void itemInserted(int index);
    virtual void itemRemoved(int index);
    void showEvent(QShowEvent *e);
    void changeEvent(QEvent *);

private:
    Q_DECLARE_PRIVATE(QToolBox)
    Q_DISABLE_COPY(QToolBox)
  
    GUI_CS_SLOT_1(Private, void _q_buttonClicked())
    GUI_CS_SLOT_2(_q_buttonClicked)

    GUI_CS_SLOT_1(Private, void _q_widgetDestroyed(QObject * un_named_arg1))
    GUI_CS_SLOT_2(_q_widgetDestroyed)
};

inline int QToolBox::addItem(QWidget *item, const QString &text)
   { return insertItem(-1, item, QIcon(), text); }

inline int QToolBox::addItem(QWidget *item, const QIcon &iconSet,const QString &text)
   { return insertItem(-1, item, iconSet, text); }

inline int QToolBox::insertItem(int index, QWidget *item, const QString &text)
   { return insertItem(index, item, QIcon(), text); }

#endif // QT_NO_TOOLBOX

QT_END_NAMESPACE

#endif // QTOOLBOX_H
