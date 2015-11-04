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

#include "qscopedpointer.h"

QT_BEGIN_NAMESPACE

/*!
    \class QScopedPointer
    \brief The QScopedPointer class stores a pointer to a dynamically allocated object, and deletes it upon destruction.
    \since 4.6
    \reentrant
    \ingroup misc

    Managing heap allocated objects manually is hard and error prone, with the
    common result that code leaks memory and is hard to maintain.
    QScopedPointer is a small utility class that heavily simplifies this by
    assigning stack-based memory ownership to heap allocations, more generally
    called resource acquisition is initialization(RAII).

    QScopedPointer guarantees that the object pointed to will get deleted when
    the current scope disappears.

    Consider this function which does heap allocations, and have various exit points:

    \snippet doc/src/snippets/code/src_corelib_tools_qscopedpointer.cpp 0

    It's encumbered by the manual delete calls. With QScopedPointer, the code
    can be simplified to:

    \snippet doc/src/snippets/code/src_corelib_tools_qscopedpointer.cpp 1

    The code the compiler generates for QScopedPointer is the same as when
    writing it manually. Code that makes use of \a delete are candidates for
    QScopedPointer usage (and if not, possibly another type of smart pointer
    such as QSharedPointer). QScopedPointer intentionally has no copy
    constructor or assignment operator, such that ownership and lifetime is
    clearly communicated.

    The const qualification on a regular C++ pointer can also be expressed with
    a QScopedPointer:

    \snippet doc/src/snippets/code/src_corelib_tools_qscopedpointer.cpp 2

    \section1 Custom cleanup handlers

    Arrays as well as pointers that have been allocated with \c malloc must
    not be deleted using \c delete. QScopedPointer's second template parameter
    can be used for custom cleanup handlers.

    The following custom cleanup handlers exist:

    \list
    \i QScopedPointerDeleter - the default, deletes the pointer using \c delete
    \i QScopedPointerArrayDeleter - deletes the pointer using \c{delete []}. Use
       this handler for pointers that were allocated with \c{new []}.
    \i QScopedPointerPodDeleter - deletes the pointer using \c{free()}. Use this
       handler for pointers that were allocated with \c{malloc()}.
    \endlist

    You can pass your own classes as handlers, provided that they have a public
    static function \c{void cleanup(T *pointer)}.

    \snippet doc/src/snippets/code/src_corelib_tools_qscopedpointer.cpp 5

    \section1 Forward Declared Pointers

    Classes that are forward declared can be used within QScopedPointer, as
    long as the destructor of the forward declared class is available whenever
    a QScopedPointer needs to clean up.

    Concretely, this means that all classes containing a QScopedPointer that
    points to a forward declared class must have non-inline constructors,
    destructors and assignment operators:

    \snippet doc/src/snippets/code/src_corelib_tools_qscopedpointer.cpp 4

    Otherwise, the compiler output a warning about not being able to destruct
    \c MyPrivateClass.

    \sa QSharedPointer
*/

/*! \typedef QScopedPointer::pointer
  \internal
 */

/*!
    \fn QScopedPointer::QScopedPointer(T *p = 0)

    Constructs this QScopedPointer instance and sets its pointer to \a p.
*/

#ifndef Q_QDOC // QTBUG-32675, qdoc can't parse rvalue refs
/*!
    \fn QScopedPointer::QScopedPointer(QScopedPointer<T, Cleanup> &&other)

    Move-constructs a QScopedPointer instance, making it point at the same
    object that \a other was pointing to. \a other is reset to point to \c{NULL}.

    \since 5.2
*/

/*!
    \fn QScopedPointer<T, Cleanup> &operator=(QScopedPointer<T, Cleanup> &&other)

    Move-assigns \a other to this QScopedPointer instance, transferring the
    ownership of the managed pointer to this instance.

    If \a other and this instance are actually the same object, this operator
    does nothing.

    Otherwise, this instance is set to point to the object \a other
    is pointing to, and \a other is set to point to \c{NULL}.
    If this instance was pointing to an object, that object is destroyed,

    \since 5.2
*/
#endif

/*!
    \fn QScopedPointer::~QScopedPointer()

    Destroys this QScopedPointer object. Delete the object its pointer points
    to.
*/

/*!
    \fn T *QScopedPointer::data() const

    Returns the value of the pointer referenced by this object. QScopedPointer
    still owns the object pointed to.
*/

/*!
    \fn T &QScopedPointer::operator*() const

    Provides access to the scoped pointer's object.

    If the contained pointer is \c null, behavior is undefined.
    \sa isNull()
*/

/*!
    \fn T *QScopedPointer::operator->() const

    Provides access to the scoped pointer's object.

    If the contained pointer is \c null, behavior is undefined.

    \sa isNull()
*/

/*!
    \fn QScopedPointer::operator bool() const

    Returns \c true if this object is not \c null. This function is suitable
    for use in \tt if-constructs, like:

    \snippet doc/src/snippets/code/src_corelib_tools_qscopedpointer.cpp 3

    \sa isNull()
*/

/*!
    \fn bool operator==(const QScopedPointer<T, Cleanup> &lhs, const QScopedPointer<T, Cleanup> &rhs)

    Equality operator. Returns true if the scoped pointers
    \a lhs and \a rhs are pointing to the same object.
    Otherwise returns false.
*/


/*!
    \fn bool operator!=(const QScopedPointer<T, Cleanup> &lhs, const QScopedPointer<T, Cleanup> &rhs)

    Inequality operator. Returns true if the scoped pointers
    \a lhs and \a rhs are \e not pointing to the same object.
    Otherwise returns false.
*/

/*!
    \fn bool QScopedPointer::isNull() const

    Returns \c true if this object is holding a pointer that is \c null.
*/

/*!
    \fn void QScopedPointer::reset(T *other = 0)

    Deletes the existing object it is pointing to if any, and sets its pointer to
    \a other. QScopedPointer now owns \a other and will delete it in its
    destructor.
*/

/*!
    \fn T *QScopedPointer::take()

    Returns the value of the pointer referenced by this object. The pointer of this
    QScopedPointer object will be reset to \c null.

    Callers of this function take ownership of the pointer.
*/

/*! \fn bool QScopedPointer::operator!() const

    Returns \c true if the pointer referenced by this object is \c null, otherwise
    returns \c false.

    \sa isNull()
*/

/*! \fn void QScopedPointer::swap(QScopedPointer<T, Cleanup> &other)
  Swap this pointer with \a other.
 */

/*!
  \class QScopedArrayPointer

  \brief The QScopedArrayPointer class stores a pointer to a
  dynamically allocated array of objects, and deletes it upon
  destruction.

  \since 4.6
  \reentrant
  \ingroup misc

  A QScopedArrayPointer is a QScopedPointer that defaults to
  deleting the object it is pointing to with the delete[] operator. It
  also features operator[] for convenience, so we can write:

  \code
    void foo()
    {
        QScopedArrayPointer<int> i(new int[10]);
        i[2] = 42;
        ...
        return; // our integer array is now deleted using delete[]
    }
  \endcode
*/

/*!
    \fn QScopedArrayPointer::QScopedArrayPointer()

    Constructs a QScopedArrayPointer instance.
*/

/*!
    \fn T *QScopedArrayPointer::operator[](int i)

    Provides access to entry \a i of the scoped pointer's array of
    objects.

    If the contained pointer is \c null, behavior is undefined.

    \sa isNull()
*/

/*!
    \fn T *QScopedArrayPointer::operator[](int i) const

    Provides access to entry \a i of the scoped pointer's array of
    objects.

    If the contained pointer is \c null, behavior is undefined.

    \sa isNull()
*/

QT_END_NAMESPACE
