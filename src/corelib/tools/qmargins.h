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

#ifndef QMARGINS_H
#define QMARGINS_H

#include <QtCore/qnamespace.h>

QT_BEGIN_NAMESPACE

class QMargins
{
public:
    QMargins();
    QMargins(int left, int top, int right, int bottom);

    bool isNull() const;

    int left() const;
    int top() const;
    int right() const;
    int bottom() const;

    void setLeft(int left);
    void setTop(int top);
    void setRight(int right);
    void setBottom(int bottom);

private:
    int m_left;
    int m_top;
    int m_right;
    int m_bottom;

    friend inline bool operator==(const QMargins &, const QMargins &);
    friend inline bool operator!=(const QMargins &, const QMargins &);
};

Q_DECLARE_TYPEINFO(QMargins, Q_MOVABLE_TYPE);

/*****************************************************************************
  QMargins inline functions
 *****************************************************************************/

inline QMargins::QMargins()
{ m_top = m_bottom = m_left = m_right = 0; }

inline QMargins::QMargins(int aleft, int atop, int aright, int abottom)
    : m_left(aleft), m_top(atop), m_right(aright), m_bottom(abottom) {}

inline bool QMargins::isNull() const
{ return m_left==0 && m_top==0 && m_right==0 && m_bottom==0; }

inline int QMargins::left() const
{ return m_left; }

inline int QMargins::top() const
{ return m_top; }

inline int QMargins::right() const
{ return m_right; }

inline int QMargins::bottom() const
{ return m_bottom; }


inline void QMargins::setLeft(int aleft)
{ m_left = aleft; }

inline void QMargins::setTop(int atop)
{ m_top = atop; }

inline void QMargins::setRight(int aright)
{ m_right = aright; }

inline void QMargins::setBottom(int abottom)
{ m_bottom = abottom; }

inline bool operator==(const QMargins &m1, const QMargins &m2)
{
    return
            m1.m_left == m2.m_left &&
            m1.m_top == m2.m_top &&
            m1.m_right == m2.m_right &&
            m1.m_bottom == m2.m_bottom;
}

inline bool operator!=(const QMargins &m1, const QMargins &m2)
{
    return
            m1.m_left != m2.m_left ||
            m1.m_top != m2.m_top ||
            m1.m_right != m2.m_right ||
            m1.m_bottom != m2.m_bottom;
}

#ifndef QT_NO_DEBUG_STREAM
Q_CORE_EXPORT QDebug operator<<(QDebug, const QMargins &);
#endif

QT_END_NAMESPACE

#endif // QMARGINS_H
