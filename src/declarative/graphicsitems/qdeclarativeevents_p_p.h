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

#ifndef QDECLARATIVEEVENTS_P_H
#define QDECLARATIVEEVENTS_P_H

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

#include <qdeclarative.h>

#include <QtCore/qobject.h>
#include <QtGui/qevent.h>

QT_BEGIN_NAMESPACE

class QDeclarativeKeyEvent : public QObject
{
    CS_OBJECT(QDeclarativeKeyEvent)
    CS_PROPERTY_READ(key, key)
    CS_PROPERTY_READ(text, text)
    CS_PROPERTY_READ(modifiers, modifiers)
    CS_PROPERTY_READ(isAutoRepeat, isAutoRepeat)
    CS_PROPERTY_READ(count, count)
    CS_PROPERTY_READ(accepted, isAccepted)
    CS_PROPERTY_WRITE(accepted, setAccepted)

public:
    QDeclarativeKeyEvent(QEvent::Type type, int key, Qt::KeyboardModifiers modifiers, const QString &text=QString(), bool autorep=false, ushort count=1)
        : event(type, key, modifiers, text, autorep, count) { event.setAccepted(false); }
    QDeclarativeKeyEvent(const QKeyEvent &ke)
        : event(ke) { event.setAccepted(false); }

    int key() const { return event.key(); }
    QString text() const { return event.text(); }
    int modifiers() const { return event.modifiers(); }
    bool isAutoRepeat() const { return event.isAutoRepeat(); }
    int count() const { return event.count(); }

    bool isAccepted() { return event.isAccepted(); }
    void setAccepted(bool accepted) { event.setAccepted(accepted); }

private:
    QKeyEvent event;
};

class QDeclarativeMouseEvent : public QObject
{
    CS_OBJECT(QDeclarativeMouseEvent)
    CS_PROPERTY_READ(x, x)
    CS_PROPERTY_READ(y, y)
    CS_PROPERTY_READ(button, button)
    CS_PROPERTY_READ(buttons, buttons)
    CS_PROPERTY_READ(modifiers, modifiers)
    CS_PROPERTY_READ(wasHeld, wasHeld)
    CS_PROPERTY_READ(isClick, isClick)
    CS_PROPERTY_READ(accepted, isAccepted)
    CS_PROPERTY_WRITE(accepted, setAccepted)

public:
    QDeclarativeMouseEvent(int x, int y, Qt::MouseButton button, Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers
                  , bool isClick=false, bool wasHeld=false)
        : _x(x), _y(y), _button(button), _buttons(buttons), _modifiers(modifiers)
          , _wasHeld(wasHeld), _isClick(isClick), _accepted(true) {}

    int x() const { return _x; }
    int y() const { return _y; }
    int button() const { return _button; }
    int buttons() const { return _buttons; }
    int modifiers() const { return _modifiers; }
    bool wasHeld() const { return _wasHeld; }
    bool isClick() const { return _isClick; }

    // only for internal usage
    void setX(int x) { _x = x; }
    void setY(int y) { _y = y; }

    bool isAccepted() { return _accepted; }
    void setAccepted(bool accepted) { _accepted = accepted; }

private:
    int _x;
    int _y;
    Qt::MouseButton _button;
    Qt::MouseButtons _buttons;
    Qt::KeyboardModifiers _modifiers;
    bool _wasHeld;
    bool _isClick;
    bool _accepted;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeKeyEvent)
QML_DECLARE_TYPE(QDeclarativeMouseEvent)

#endif // QDECLARATIVEEVENTS_P_H
