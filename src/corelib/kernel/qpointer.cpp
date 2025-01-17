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

/*!
    \class QPointer
    \brief The QPointer class is a template class that provides guarded pointers to QObject.

    \ingroup objectmodel


    A guarded pointer, QPointer<T>, behaves like a normal C++
    pointer \c{T *}, except that it is automatically set to 0 when the
    referenced object is destroyed (unlike normal C++ pointers, which
    become "dangling pointers" in such cases). \c T must be a
    subclass of QObject.

    Guarded pointers are useful whenever you need to store a pointer
    to a QObject that is owned by someone else, and therefore might be
    destroyed while you still hold a reference to it. You can safely
    test the pointer for validity.

    Qt also provides QSharedPointer, an implementation of a reference-counted
    shared pointer object, which can be used to maintain a collection of
    references to an individual pointer.

    Example:

    \snippet doc/src/snippets/pointer/pointer.cpp 0
    \dots
    \snippet doc/src/snippets/pointer/pointer.cpp 1
    \snippet doc/src/snippets/pointer/pointer.cpp 2

    If the QLabel is deleted in the meantime, the \c label variable
    will hold 0 instead of an invalid address, and the last line will
    never be executed.

    The functions and operators available with a QPointer are the
    same as those available with a normal unguarded pointer, except
    the pointer arithmetic operators (\c{+}, \c{-}, \c{++}, and
    \c{--}), which are normally used only with arrays of objects.

    Use QPointers like normal pointers and you will not need to read
    this class documentation.

    For creating guarded pointers, you can construct or assign to them
    from a T* or from another guarded pointer of the same type. You
    can compare them with each other using operator==() and
    operator!=(), or test for 0 with isNull(). You can dereference
    them using either the \c *x or the \c x->member notation.

    A guarded pointer will automatically cast to a \c T *, so you can
    freely mix guarded and unguarded pointers. This means that if you
    have a QPointer<QWidget>, you can pass it to a function that
    requires a QWidget *. For this reason, it is of little value to
    declare functions to take a QPointer as a parameter; just use
    normal pointers. Use a QPointer when you are storing a pointer
    over time.

    Note that class \c T must inherit QObject, or a compilation or
    link error will result.

    \sa QSharedPointer, QWeakPointer, QObject, QObjectCleanupHandler
*/

/*!
    \fn QPointer::QPointer()

    Constructs a 0 guarded pointer.

    \sa isNull()
*/

/*!
    \fn QPointer::QPointer(T* p)

    Constructs a guarded pointer that points to same object that \a p
    points to.
*/

/*!
    \fn QPointer::QPointer(const QPointer<T> &p)

    Copies one guarded pointer from another. The constructed guarded
    pointer points to the same object that \a p points to (which may
    be 0).
*/

/*!
    \fn QPointer::~QPointer()

    Destroys the guarded pointer. Just like a normal pointer,
    destroying a guarded pointer does \e not destroy the object being
    pointed to.
*/

/*!
    \fn QPointer<T>& QPointer::operator=(const QPointer<T> &p)

    Assignment operator. This guarded pointer will now point to the
    same object that \a p points to.
*/

/*!
    \fn QPointer<T> & QPointer::operator=(T* p)

    Assignment operator. This guarded pointer will now point to the
    same object that \a p points to.
*/

/*!
    \fn T* QPointer::data() const
    \since 4.4

    Returns the pointer to the object being guarded.
*/

/*!
    \fn bool QPointer::isNull() const

    Returns \c true if the referenced object has been destroyed or if
    there is no referenced object; otherwise returns false.
*/

/*!
    \fn void QPointer::clear()

    Clears this QPointer object.

    \sa isNull()
*/

/*!
    \fn T* QPointer::operator->() const

    Overloaded arrow operator; implements pointer semantics. Just use
    this operator as you would with a normal C++ pointer.
*/

/*!
    \fn T& QPointer::operator*() const

    Dereference operator; implements pointer semantics. Just use this
    operator as you would with a normal C++ pointer.
*/

/*!
    \fn QPointer::operator T*() const

    Cast operator; implements pointer semantics. Because of this
    function you can pass a QPointer\<T\> to a function where a T*
    is required.
*/

/*!
    \fn bool operator==(const T *o, const QPointer<T> &p)
    \relates QPointer

    Equality operator. Returns true if \a o and the guarded
    pointer \a p are pointing to the same object, otherwise
    returns false.

*/
/*!
    \fn bool operator==(const QPointer<T> &p, const T *o)
    \relates QPointer

    Equality operator. Returns true if \a o and the guarded
    pointer \a p are pointing to the same object, otherwise
    returns false.

*/
/*!
    \fn bool operator==(T *o, const QPointer<T> &p)
    \relates QPointer

    Equality operator. Returns true if \a o and the guarded
    pointer \a p are pointing to the same object, otherwise
    returns false.

*/
/*!
    \fn bool operator==(const QPointer<T> &p, T *o)
    \relates QPointer

    Equality operator. Returns true if \a o and the guarded
    pointer \a p are pointing to the same object, otherwise
    returns false.

*/
/*!
    \fn bool operator==(const QPointer<T> &p1, const QPointer<T> &p2)
    \relates QPointer

    Equality operator. Returns true if the guarded pointers \a p1 and \a p2
    are pointing to the same object, otherwise
    returns false.

*/


/*!
    \fn bool operator!=(const T *o, const QPointer<T> &p)
    \relates QPointer

    Inequality operator. Returns true if \a o and the guarded
    pointer \a p are not pointing to the same object, otherwise
    returns false.
*/
/*!
    \fn bool operator!=(const QPointer<T> &p, const T *o)
    \relates QPointer

    Inequality operator. Returns true if \a o and the guarded
    pointer \a p are not pointing to the same object, otherwise
    returns false.
*/
/*!
    \fn bool operator!=(T *o, const QPointer<T> &p)
    \relates QPointer

    Inequality operator. Returns true if \a o and the guarded
    pointer \a p are not pointing to the same object, otherwise
    returns false.
*/
/*!
    \fn bool operator!=(const QPointer<T> &p, T *o)
    \relates QPointer

    Inequality operator. Returns true if \a o and the guarded
    pointer \a p are not pointing to the same object, otherwise
    returns false.
*/
/*!
    \fn bool operator!=(const QPointer<T> &p1, const QPointer<T> &p2)
    \relates QPointer

    Inequality operator. Returns true if  the guarded pointers \a p1 and
    \a p2 are not pointing to the same object, otherwise
    returns false.
*/
