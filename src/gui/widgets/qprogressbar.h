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

#ifndef QPROGRESSBAR_H
#define QPROGRESSBAR_H

#include <QtGui/qframe.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_PROGRESSBAR

class QProgressBarPrivate;
class QStyleOptionProgressBar;

class Q_GUI_EXPORT QProgressBar : public QWidget
{
    CS_OBJECT(QProgressBar)

    GUI_CS_ENUM(Direction)

    GUI_CS_PROPERTY_READ(minimum, minimum)
    GUI_CS_PROPERTY_WRITE(minimum, setMinimum)

    GUI_CS_PROPERTY_READ(maximum, maximum)
    GUI_CS_PROPERTY_WRITE(maximum, setMaximum)

    GUI_CS_PROPERTY_READ(text, text)

    GUI_CS_PROPERTY_READ(value, value)
    GUI_CS_PROPERTY_WRITE(value, setValue)
    GUI_CS_PROPERTY_NOTIFY(value, valueChanged)

    GUI_CS_PROPERTY_READ(alignment, alignment)
    GUI_CS_PROPERTY_WRITE(alignment, setAlignment)

    GUI_CS_PROPERTY_READ(textVisible, isTextVisible)
    GUI_CS_PROPERTY_WRITE(textVisible, setTextVisible)

    GUI_CS_PROPERTY_READ(orientation, orientation)
    GUI_CS_PROPERTY_WRITE(orientation, setOrientation)

    GUI_CS_PROPERTY_READ(invertedAppearance, invertedAppearance)   
    GUI_CS_PROPERTY_WRITE(invertedAppearance, setInvertedAppearance) 

    GUI_CS_PROPERTY_READ(textDirection, textDirection)
    GUI_CS_PROPERTY_WRITE(textDirection, setTextDirection)

    GUI_CS_PROPERTY_READ(format, format)
    GUI_CS_PROPERTY_WRITE(format, setFormat)

public:
    enum Direction { TopToBottom, BottomToTop };

    explicit QProgressBar(QWidget *parent = 0);

    int minimum() const;
    int maximum() const;

    int value() const;

    virtual QString text() const;
    void setTextVisible(bool visible);
    bool isTextVisible() const;

    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment alignment);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    Qt::Orientation orientation() const;

    void setInvertedAppearance(bool invert);
    bool invertedAppearance() const;    
  
    void setTextDirection(QProgressBar::Direction textDirection);
    QProgressBar::Direction textDirection() const;
   
    void setFormat(const QString &format);
    QString format() const;

    GUI_CS_SLOT_1(Public, void reset())
    GUI_CS_SLOT_2(reset) 

    GUI_CS_SLOT_1(Public, void setRange(int minimum,int maximum))
    GUI_CS_SLOT_2(setRange) 

    GUI_CS_SLOT_1(Public, void setMinimum(int minimum))
    GUI_CS_SLOT_2(setMinimum) 

    GUI_CS_SLOT_1(Public, void setMaximum(int maximum))
    GUI_CS_SLOT_2(setMaximum) 

    GUI_CS_SLOT_1(Public, void setValue(int value))
    GUI_CS_SLOT_2(setValue) 

    GUI_CS_SLOT_1(Public, void setOrientation(Qt::Orientation un_named_arg1))
    GUI_CS_SLOT_2(setOrientation) 

    GUI_CS_SIGNAL_1(Public, void valueChanged(int value))
    GUI_CS_SIGNAL_2(valueChanged,value) 

protected:
    bool event(QEvent *e);
    void paintEvent(QPaintEvent *);
    void initStyleOption(QStyleOptionProgressBar *option) const;

private:
    Q_DECLARE_PRIVATE(QProgressBar)
    Q_DISABLE_COPY(QProgressBar)
};

#endif // QT_NO_PROGRESSBAR

QT_END_NAMESPACE

#endif // QPROGRESSBAR_H
