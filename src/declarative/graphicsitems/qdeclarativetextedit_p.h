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

#ifndef QDECLARATIVETEXTEDIT_H
#define QDECLARATIVETEXTEDIT_H

#include "private/qdeclarativetext_p.h"
#include "private/qdeclarativeimplicitsizeitem_p.h"

#include <QtGui/qtextdocument.h>
#include <QtGui/qtextoption.h>
#include <QtGui/qtextcursor.h>
#include <QtGui/qtextformat.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QDeclarativeTextEditPrivate;

class QDeclarativeTextEdit : public QDeclarativeImplicitSizePaintedItem
{
    CS_OBJECT(QDeclarativeTextEdit)

    CS_ENUM(VAlignment)
    CS_ENUM(HAlignment)
    CS_ENUM(TextFormat)
    CS_ENUM(WrapMode)
    CS_ENUM(SelectionMode)

    CS_PROPERTY_READ(text, text)
    CS_PROPERTY_WRITE(text, setText)
    CS_PROPERTY_NOTIFY(text, textChanged)
    CS_PROPERTY_READ(color, color)
    CS_PROPERTY_WRITE(color, setColor)
    CS_PROPERTY_NOTIFY(color, colorChanged)
    CS_PROPERTY_READ(selectionColor, selectionColor)
    CS_PROPERTY_WRITE(selectionColor, setSelectionColor)
    CS_PROPERTY_NOTIFY(selectionColor, selectionColorChanged)
    CS_PROPERTY_READ(selectedTextColor, selectedTextColor)
    CS_PROPERTY_WRITE(selectedTextColor, setSelectedTextColor)
    CS_PROPERTY_NOTIFY(selectedTextColor, selectedTextColorChanged)
    CS_PROPERTY_READ(font, font)
    CS_PROPERTY_WRITE(font, setFont)
    CS_PROPERTY_NOTIFY(font, fontChanged)
    CS_PROPERTY_READ(horizontalAlignment, hAlign)
    CS_PROPERTY_WRITE(horizontalAlignment, setHAlign)
    CS_PROPERTY_RESET(horizontalAlignment, resetHAlign)
    CS_PROPERTY_NOTIFY(horizontalAlignment, horizontalAlignmentChanged)
    CS_PROPERTY_READ(verticalAlignment, vAlign)
    CS_PROPERTY_WRITE(verticalAlignment, setVAlign)
    CS_PROPERTY_NOTIFY(verticalAlignment, verticalAlignmentChanged)
    CS_PROPERTY_READ(wrapMode, wrapMode)
    CS_PROPERTY_WRITE(wrapMode, setWrapMode)
    CS_PROPERTY_NOTIFY(wrapMode, wrapModeChanged)
    CS_PROPERTY_READ(lineCount, lineCount)
    CS_PROPERTY_NOTIFY(lineCount, lineCountChanged)
    CS_PROPERTY_REVISION(lineCount, 1)
    CS_PROPERTY_READ(paintedWidth, paintedWidth)
    CS_PROPERTY_NOTIFY(paintedWidth, paintedSizeChanged)
    CS_PROPERTY_READ(paintedHeight, paintedHeight)
    CS_PROPERTY_NOTIFY(paintedHeight, paintedSizeChanged)
    CS_PROPERTY_READ(textFormat, textFormat)
    CS_PROPERTY_WRITE(textFormat, setTextFormat)
    CS_PROPERTY_NOTIFY(textFormat, textFormatChanged)
    CS_PROPERTY_READ(readOnly, isReadOnly)
    CS_PROPERTY_WRITE(readOnly, setReadOnly)
    CS_PROPERTY_NOTIFY(readOnly, readOnlyChanged)
    CS_PROPERTY_READ(cursorVisible, isCursorVisible)
    CS_PROPERTY_WRITE(cursorVisible, setCursorVisible)
    CS_PROPERTY_NOTIFY(cursorVisible, cursorVisibleChanged)
    CS_PROPERTY_READ(cursorPosition, cursorPosition)
    CS_PROPERTY_WRITE(cursorPosition, setCursorPosition)
    CS_PROPERTY_NOTIFY(cursorPosition, cursorPositionChanged)
    CS_PROPERTY_READ(cursorRectangle, cursorRectangle)
    CS_PROPERTY_NOTIFY(cursorRectangle, cursorRectangleChanged)
    CS_PROPERTY_READ(cursorDelegate, cursorDelegate)
    CS_PROPERTY_WRITE(cursorDelegate, setCursorDelegate)
    CS_PROPERTY_NOTIFY(cursorDelegate, cursorDelegateChanged)
    CS_PROPERTY_READ(selectionStart, selectionStart)
    CS_PROPERTY_NOTIFY(selectionStart, selectionStartChanged)
    CS_PROPERTY_READ(selectionEnd, selectionEnd)
    CS_PROPERTY_NOTIFY(selectionEnd, selectionEndChanged)
    CS_PROPERTY_READ(selectedText, selectedText)
    CS_PROPERTY_NOTIFY(selectedText, selectionChanged)
    CS_PROPERTY_READ(activeFocusOnPress, focusOnPress)
    CS_PROPERTY_WRITE(activeFocusOnPress, setFocusOnPress)
    CS_PROPERTY_NOTIFY(activeFocusOnPress, activeFocusOnPressChanged)
    CS_PROPERTY_READ(persistentSelection, persistentSelection)
    CS_PROPERTY_WRITE(persistentSelection, setPersistentSelection)
    CS_PROPERTY_NOTIFY(persistentSelection, persistentSelectionChanged)
    CS_PROPERTY_READ(textMargin, textMargin)
    CS_PROPERTY_WRITE(textMargin, setTextMargin)
    CS_PROPERTY_NOTIFY(textMargin, textMarginChanged)
    CS_PROPERTY_READ(inputMethodHints, inputMethodHints)
    CS_PROPERTY_WRITE(inputMethodHints, setInputMethodHints)
    CS_PROPERTY_READ(selectByMouse, selectByMouse)
    CS_PROPERTY_WRITE(selectByMouse, setSelectByMouse)
    CS_PROPERTY_NOTIFY(selectByMouse, selectByMouseChanged)
    CS_PROPERTY_READ(mouseSelectionMode, mouseSelectionMode)
    CS_PROPERTY_WRITE(mouseSelectionMode, setMouseSelectionMode)
    CS_PROPERTY_NOTIFY(mouseSelectionMode, mouseSelectionModeChanged)
    CS_PROPERTY_REVISION(mouseSelectionMode, 1)
    CS_PROPERTY_READ(canPaste, canPaste)
    CS_PROPERTY_NOTIFY(canPaste, canPasteChanged)
    CS_PROPERTY_REVISION(canPaste, 1)
    CS_PROPERTY_READ(inputMethodComposing, isInputMethodComposing)
    CS_PROPERTY_NOTIFY(inputMethodComposing, inputMethodComposingChanged)
    CS_PROPERTY_REVISION(inputMethodComposing, 1)

public:
    QDeclarativeTextEdit(QDeclarativeItem *parent=0);

    enum HAlignment {
        AlignLeft = Qt::AlignLeft,
        AlignRight = Qt::AlignRight,
        AlignHCenter = Qt::AlignHCenter,
        AlignJustify = Qt::AlignJustify // ### VERSIONING: Only in QtQuick 1.1
    };

    enum VAlignment {
        AlignTop = Qt::AlignTop,
        AlignBottom = Qt::AlignBottom,
        AlignVCenter = Qt::AlignVCenter
    };

    enum TextFormat {
        PlainText = Qt::PlainText,
        RichText = Qt::RichText,
        AutoText = Qt::AutoText
    };

    enum WrapMode { NoWrap = QTextOption::NoWrap,
                    WordWrap = QTextOption::WordWrap,
                    WrapAnywhere = QTextOption::WrapAnywhere,
                    WrapAtWordBoundaryOrAnywhere = QTextOption::WrapAtWordBoundaryOrAnywhere, // COMPAT
                    Wrap = QTextOption::WrapAtWordBoundaryOrAnywhere
                  };

    enum SelectionMode {
        SelectCharacters,
        SelectWords
    };
   
    CS_INVOKABLE_METHOD_1(Public,void openSoftwareInputPanel())
    CS_INVOKABLE_METHOD_2(openSoftwareInputPanel)
   
    CS_INVOKABLE_METHOD_1(Public, void closeSoftwareInputPanel())
    CS_INVOKABLE_METHOD_2(closeSoftwareInputPanel)
      
    CS_INVOKABLE_METHOD_1(Public,QRectF positionToRectangle(int) const)
    CS_INVOKABLE_METHOD_2(positionToRectangle)
      
    CS_INVOKABLE_METHOD_1(Public, int positionAt(int x, int y) const)
    CS_INVOKABLE_METHOD_2(positionAt)
   
    CS_INVOKABLE_METHOD_1(Public, void moveCursorSelection(int pos))
    CS_INVOKABLE_METHOD_2(moveCursorSelection)    
   
    CS_INVOKABLE_METHOD_1(Public,void moveCursorSelection(int pos, SelectionMode mode) )
    CS_INVOKABLE_METHOD_2(moveCursorSelection)
    CS_REVISION(moveCursorSelection,1)   

    QString text() const;
    void setText(const QString &);

    TextFormat textFormat() const;
    void setTextFormat(TextFormat format);

    QFont font() const;
    void setFont(const QFont &font);

    QColor color() const;
    void setColor(const QColor &c);

    QColor selectionColor() const;
    void setSelectionColor(const QColor &c);

    QColor selectedTextColor() const;
    void setSelectedTextColor(const QColor &c);

    HAlignment hAlign() const;
    void setHAlign(HAlignment align);
    void resetHAlign();
    HAlignment effectiveHAlign() const;

    VAlignment vAlign() const;
    void setVAlign(VAlignment align);

    WrapMode wrapMode() const;
    void setWrapMode(WrapMode w);

    int lineCount() const;

    bool isCursorVisible() const;
    void setCursorVisible(bool on);

    int cursorPosition() const;
    void setCursorPosition(int pos);

    QDeclarativeComponent* cursorDelegate() const;
    void setCursorDelegate(QDeclarativeComponent*);

    int selectionStart() const;
    int selectionEnd() const;

    QString selectedText() const;

    bool focusOnPress() const;
    void setFocusOnPress(bool on);

    bool persistentSelection() const;
    void setPersistentSelection(bool on);

    qreal textMargin() const;
    void setTextMargin(qreal margin);

    bool selectByMouse() const;
    void setSelectByMouse(bool);

    SelectionMode mouseSelectionMode() const;
    void setMouseSelectionMode(SelectionMode mode);

    bool canPaste() const;

    virtual void componentComplete();
   
    void setReadOnly(bool);
    bool isReadOnly() const;

    void setTextInteractionFlags(Qt::TextInteractionFlags flags);
    Qt::TextInteractionFlags textInteractionFlags() const;

    QRect cursorRectangle() const;

    QVariant inputMethodQuery(Qt::InputMethodQuery property) const;

    qreal paintedWidth() const;
    qreal paintedHeight() const;
 

    QRectF boundingRect() const;

    bool isInputMethodComposing() const;

    CS_SIGNAL_1(Public, void textChanged(const QString & un_named_arg1))
    CS_SIGNAL_2(textChanged,un_named_arg1) 
    CS_SIGNAL_1(Public, void paintedSizeChanged())
    CS_SIGNAL_2(paintedSizeChanged) 
    CS_SIGNAL_1(Public, void cursorPositionChanged())
    CS_SIGNAL_2(cursorPositionChanged) 
    CS_SIGNAL_1(Public, void cursorRectangleChanged())
    CS_SIGNAL_2(cursorRectangleChanged) 
    CS_SIGNAL_1(Public, void selectionStartChanged())
    CS_SIGNAL_2(selectionStartChanged) 
    CS_SIGNAL_1(Public, void selectionEndChanged())
    CS_SIGNAL_2(selectionEndChanged) 
    CS_SIGNAL_1(Public, void selectionChanged())
    CS_SIGNAL_2(selectionChanged) 
    CS_SIGNAL_1(Public, void colorChanged(const QColor & color))
    CS_SIGNAL_2(colorChanged,color) 
    CS_SIGNAL_1(Public, void selectionColorChanged(const QColor & color))
    CS_SIGNAL_2(selectionColorChanged,color) 
    CS_SIGNAL_1(Public, void selectedTextColorChanged(const QColor & color))
    CS_SIGNAL_2(selectedTextColorChanged,color) 
    CS_SIGNAL_1(Public, void fontChanged(const QFont & font))
    CS_SIGNAL_2(fontChanged,font) 
    CS_SIGNAL_1(Public, void horizontalAlignmentChanged(HAlignment alignment))
    CS_SIGNAL_2(horizontalAlignmentChanged,alignment) 
    CS_SIGNAL_1(Public, void verticalAlignmentChanged(VAlignment alignment))
    CS_SIGNAL_2(verticalAlignmentChanged,alignment) 
    CS_SIGNAL_1(Public, void wrapModeChanged())
    CS_SIGNAL_2(wrapModeChanged) 
    CS_SIGNAL_1(Public, void lineCountChanged())
    CS_SIGNAL_2(lineCountChanged) 
    CS_SIGNAL_1(Public, void textFormatChanged(TextFormat textFormat))
    CS_SIGNAL_2(textFormatChanged,textFormat) 
    CS_SIGNAL_1(Public, void readOnlyChanged(bool isReadOnly))
    CS_SIGNAL_2(readOnlyChanged,isReadOnly) 
    CS_SIGNAL_1(Public, void cursorVisibleChanged(bool isCursorVisible))
    CS_SIGNAL_2(cursorVisibleChanged,isCursorVisible) 
    CS_SIGNAL_1(Public, void cursorDelegateChanged())
    CS_SIGNAL_2(cursorDelegateChanged) 
    CS_SIGNAL_1(Public, void activeFocusOnPressChanged(bool activeFocusOnPressed))
    CS_SIGNAL_2(activeFocusOnPressChanged,activeFocusOnPressed) 
    CS_SIGNAL_1(Public, void persistentSelectionChanged(bool isPersistentSelection))
    CS_SIGNAL_2(persistentSelectionChanged,isPersistentSelection) 
    CS_SIGNAL_1(Public, void textMarginChanged(qreal textMargin))
    CS_SIGNAL_2(textMarginChanged,textMargin) 
    CS_SIGNAL_1(Public, void selectByMouseChanged(bool selectByMouse))
    CS_SIGNAL_2(selectByMouseChanged,selectByMouse) 

    CS_SIGNAL_1(Public, void mouseSelectionModeChanged(SelectionMode mode))
    CS_SIGNAL_2(mouseSelectionModeChanged,mode) 
    CS_REVISION(mouseSelectionModeChanged,mode,1) 

    CS_SIGNAL_1(Public, void linkActivated(const QString & link))
    CS_SIGNAL_2(linkActivated,link) 
    CS_REVISION(linkActivated,link,1) 

    CS_SIGNAL_1(Public, void canPasteChanged())
    CS_SIGNAL_2(canPasteChanged) 
    CS_REVISION(canPasteChanged,1) 

    CS_SIGNAL_1(Public, void inputMethodComposingChanged())
    CS_SIGNAL_2(inputMethodComposingChanged) 
    CS_REVISION(inputMethodComposingChanged,1)

    CS_SLOT_1(Public, void selectAll())
    CS_SLOT_2(selectAll) 
    CS_SLOT_1(Public, void selectWord())
    CS_SLOT_2(selectWord) 
    CS_SLOT_1(Public, void select(int start,int end))
    CS_SLOT_2(select) 

    CS_SLOT_1(Public, void deselect())
    CS_SLOT_2(deselect) 
    CS_REVISION(deselect,1)

    CS_SLOT_1(Public, bool isRightToLeft(int start,int end))
    CS_SLOT_2(isRightToLeft) 
    CS_REVISION(isRightToLeft,1)

#ifndef QT_NO_CLIPBOARD
    CS_SLOT_1(Public, void cut())
    CS_SLOT_2(cut) 
    CS_SLOT_1(Public, void copy())
    CS_SLOT_2(copy) 
    CS_SLOT_1(Public, void paste())
    CS_SLOT_2(paste) 
#endif

protected:
    virtual void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

    bool event(QEvent *);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void focusInEvent(QFocusEvent *event);

    // mouse filter?
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    void inputMethodEvent(QInputMethodEvent *e);

    void drawContents(QPainter *, const QRect &);

private :
    CS_SLOT_1(Private, void updateImgCache(const QRectF & rect))
    CS_SLOT_2(updateImgCache) 
    CS_SLOT_1(Private, void q_textChanged())
    CS_SLOT_2(q_textChanged) 
    CS_SLOT_1(Private, void updateSelectionMarkers())
    CS_SLOT_2(updateSelectionMarkers) 
    CS_SLOT_1(Private, void moveCursorDelegate())
    CS_SLOT_2(moveCursorDelegate) 
    CS_SLOT_1(Private, void loadCursorDelegate())
    CS_SLOT_2(loadCursorDelegate) 
    CS_SLOT_1(Private, void q_canPasteChanged())
    CS_SLOT_2(q_canPasteChanged) 

    void updateSize();
    void updateTotalLines();
   
    Q_DISABLE_COPY(QDeclarativeTextEdit)
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), QDeclarativeTextEdit)
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeTextEdit)

QT_END_HEADER

#endif
