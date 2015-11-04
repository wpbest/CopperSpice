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

#include "qplatformdefs.h"
#include "qmutex.h"
#include <qdebug.h>
#include "qatomic.h"
#include "qelapsedtimer.h"
#include "qthread.h"
#include "qmutex_p.h"

#ifndef Q_OS_LINUX
#include "qfreelist_p.h"
#endif

QT_BEGIN_NAMESPACE

/*!
    \class QBasicMutex
    \brief QMutex POD
    \internal

    \ingroup thread

    - Can be used as global static object.
    - Always non-recursive
    - Do not use tryLock with timeout > 0, else you can have a leak (see the ~QMutex destructor)
*/


/*!
    \class QMutex
    \brief The QMutex class provides access serialization between threads.

    \threadsafe

    \ingroup thread

    The purpose of a QMutex is to protect an object, data structure or
    section of code so that only one thread can access it at a time
    (this is similar to the Java \c synchronized keyword). It is
    usually best to use a mutex with a QMutexLocker since this makes
    it easy to ensure that locking and unlocking are performed
    consistently.

    For example, say there is a method that prints a message to the
    user on two lines:

    \snippet doc/src/snippets/code/src_corelib_thread_qmutex.cpp 0

    If these two methods are called in succession, the following happens:

    \snippet doc/src/snippets/code/src_corelib_thread_qmutex.cpp 1

    If these two methods are called simultaneously from two threads then the
    following sequence could result:

    \snippet doc/src/snippets/code/src_corelib_thread_qmutex.cpp 2

    If we add a mutex, we should get the result we want:

    \snippet doc/src/snippets/code/src_corelib_thread_qmutex.cpp 3

    Then only one thread can modify \c number at any given time and
    the result is correct. This is a trivial example, of course, but
    applies to any other case where things need to happen in a
    particular sequence.

    When you call lock() in a thread, other threads that try to call
    lock() in the same place will block until the thread that got the
    lock calls unlock(). A non-blocking alternative to lock() is
    tryLock().

    \sa QMutexLocker, QReadWriteLock, QSemaphore, QWaitCondition
*/

/*!
    \enum QMutex::RecursionMode

    \value Recursive  In this mode, a thread can lock the same mutex
                      multiple times and the mutex won't be unlocked
                      until a corresponding number of unlock() calls
                      have been made.

    \value NonRecursive  In this mode, a thread may only lock a mutex
                         once.

    \sa QMutex()
*/

/*!
    Constructs a new mutex. The mutex is created in an unlocked state.

    If \a mode is QMutex::Recursive, a thread can lock the same mutex
    multiple times and the mutex won't be unlocked until a
    corresponding number of unlock() calls have been made. The
    default is QMutex::NonRecursive.

    \sa lock(), unlock()
*/
QMutex::QMutex(RecursionMode mode)
{
    d_ptr.store(mode == Recursive ? new QRecursiveMutexPrivate : 0);
}

/*!
    Destroys the mutex.

    \warning Destroying a locked mutex may result in undefined behavior.
*/
QMutex::~QMutex()
{
    QMutexData *d = d_ptr.load();
    if (quintptr(d) > 0x3 && d->recursive) {
        delete static_cast<QRecursiveMutexPrivate *>(d);
    } else if (d) {
#ifndef Q_OS_LINUX
        if (d != dummyLocked() && static_cast<QMutexPrivate *>(d)->possiblyUnlocked.load()
            && tryLock()) {
            unlock();
            return;
        }
#endif
        qWarning("QMutex: destroying locked mutex");
    }
}

/*! \fn void QMutex::lock()
    Locks the mutex. If another thread has locked the mutex then this
    call will block until that thread has unlocked it.

    Calling this function multiple times on the same mutex from the
    same thread is allowed if this mutex is a
    \l{QMutex::Recursive}{recursive mutex}. If this mutex is a
    \l{QMutex::NonRecursive}{non-recursive mutex}, this function will
    \e dead-lock when the mutex is locked recursively.

    \sa unlock()
*/

/*!\fn bool QMutex::trylock()
    Attempts to lock the mutex. If the lock was obtained, this function
    returns true. If another thread has locked the mutex, this
    function returns false immediately.

    If the lock was obtained, the mutex must be unlocked with unlock()
    before another thread can successfully lock it.

    Calling this function multiple times on the same mutex from the
    same thread is allowed if this mutex is a
    \l{QMutex::Recursive}{recursive mutex}. If this mutex is a
    \l{QMutex::NonRecursive}{non-recursive mutex}, this function will
    \e always return false when attempting to lock the mutex
    recursively.

    \sa lock(), unlock()
*/

/*! \fn bool QMutex::tryLock(int timeout)
     \overload

    Attempts to lock the mutex. This function returns true if the lock
    was obtained; otherwise it returns false. If another thread has
    locked the mutex, this function will wait for at most \a timeout
    milliseconds for the mutex to become available.

    Note: Passing a negative number as the \a timeout is equivalent to
    calling lock(), i.e. this function will wait forever until mutex
    can be locked if \a timeout is negative.

    If the lock was obtained, the mutex must be unlocked with unlock()
    before another thread can successfully lock it.

    Calling this function multiple times on the same mutex from the
    same thread is allowed if this mutex is a
    \l{QMutex::Recursive}{recursive mutex}. If this mutex is a
    \l{QMutex::NonRecursive}{non-recursive mutex}, this function will
    \e always return false when attempting to lock the mutex
    recursively.

    \sa lock(), unlock()
*/


/*! \fn void QMutex::unlock()
    Unlocks the mutex. Attempting to unlock a mutex in a different
    thread to the one that locked it results in an error. Unlocking a
    mutex that is not locked results in undefined behavior.

    \sa lock()
*/

/*!
    \fn void QMutex::isRecursive()
    \since 5.0

    Returns true if the mutex is recursive

*/
bool QBasicMutex::isRecursive() {
    QMutexData *d = d_ptr.load();
    if (quintptr(d) <= 0x3)
        return false;
    return d->recursive;
}


/*!
    \class QMutexLocker
    \brief The QMutexLocker class is a convenience class that simplifies
    locking and unlocking mutexes.

    \threadsafe

    \ingroup thread

    Locking and unlocking a QMutex in complex functions and
    statements or in exception handling code is error-prone and
    difficult to debug. QMutexLocker can be used in such situations
    to ensure that the state of the mutex is always well-defined.

    QMutexLocker should be created within a function where a
    QMutex needs to be locked. The mutex is locked when QMutexLocker
    is created. You can unlock and relock the mutex with \c unlock()
    and \c relock(). If locked, the mutex will be unlocked when the
    QMutexLocker is destroyed.

    For example, this complex function locks a QMutex upon entering
    the function and unlocks the mutex at all the exit points:

    \snippet doc/src/snippets/code/src_corelib_thread_qmutex.cpp 4

    This example function will get more complicated as it is
    developed, which increases the likelihood that errors will occur.

    Using QMutexLocker greatly simplifies the code, and makes it more
    readable:

    \snippet doc/src/snippets/code/src_corelib_thread_qmutex.cpp 5

    Now, the mutex will always be unlocked when the QMutexLocker
    object is destroyed (when the function returns since \c locker is
    an auto variable).

    The same principle applies to code that throws and catches
    exceptions. An exception that is not caught in the function that
    has locked the mutex has no way of unlocking the mutex before the
    exception is passed up the stack to the calling function.

    QMutexLocker also provides a \c mutex() member function that returns
    the mutex on which the QMutexLocker is operating. This is useful
    for code that needs access to the mutex, such as
    QWaitCondition::wait(). For example:

    \snippet doc/src/snippets/code/src_corelib_thread_qmutex.cpp 6

    \sa QReadLocker, QWriteLocker, QMutex
*/

/*!
    \fn QMutexLocker::QMutexLocker(QMutex *mutex)

    Constructs a QMutexLocker and locks \a mutex. The mutex will be
    unlocked when the QMutexLocker is destroyed. If \a mutex is zero,
    QMutexLocker does nothing.

    \sa QMutex::lock()
*/

/*!
    \fn QMutexLocker::~QMutexLocker()

    Destroys the QMutexLocker and unlocks the mutex that was locked
    in the constructor.

    \sa QMutex::unlock()
*/

/*!
    \fn QMutex *QMutexLocker::mutex() const

    Returns a pointer to the mutex that was locked in the
    constructor.
*/

/*!
    \fn void QMutexLocker::unlock()

    Unlocks this mutex locker. You can use \c relock() to lock
    it again. It does not need to be locked when destroyed.

    \sa relock()
*/

/*!
    \fn void QMutexLocker::relock()

    Relocks an unlocked mutex locker.

    \sa unlock()
*/

#ifndef Q_OS_LINUX //linux implementation is in qmutex_linux.cpp
/*!
    \internal helper for lock()
 */
bool QBasicMutex::lockInternal(int timeout)
{
    while (!fastTryLock()) {
        QMutexData *copy = d_ptr.loadAcquire();
        if (!copy) // if d is 0, the mutex is unlocked
            continue;

        if (copy == dummyLocked()) {
            if (timeout == 0)
                return false;
            QMutexPrivate *newD = QMutexPrivate::allocate();
            if (!d_ptr.testAndSetOrdered(dummyLocked(), newD)) {
                //Either the mutex is already unlocked, or another thread already set it.
                newD->deref();
                continue;
            }
            copy = newD;
            //the d->refCount is already 1 the deref will occurs when we unlock
        } else if (copy->recursive) {
             return static_cast<QRecursiveMutexPrivate *>(copy)->lock(timeout);
        }

        QMutexPrivate *d = static_cast<QMutexPrivate *>(copy);
        if (timeout == 0 && !d->possiblyUnlocked.load())
            return false;

        if (!d->ref())
            continue; //that QMutexData was already released

        if (d != d_ptr.loadAcquire()) {
            //Either the mutex is already unlocked, or relocked with another mutex
            d->deref();
            continue;
        }

        int old_waiters;
        do {
            old_waiters = d->waiters.load();
            if (old_waiters == -QMutexPrivate::BigNumber) {
                // we are unlocking, and the thread that unlocks is about to change d to 0
                // we try to aquire the mutex by changing to dummyLocked()
                if (d_ptr.testAndSetAcquire(d, dummyLocked())) {
                    // Mutex acquired
                    d->deref();
                    return true;
                } else {
                    Q_ASSERT(d != d_ptr.load()); //else testAndSetAcquire should have succeeded
                    // Mutex is likely to bo 0, we should continue the outer-loop,
                    //  set old_waiters to the magic value of BigNumber
                    old_waiters = QMutexPrivate::BigNumber;
                    break;
                }
            }
        } while (!d->waiters.testAndSetRelaxed(old_waiters, old_waiters + 1));

        if (d != d_ptr.loadAcquire()) {
            // Mutex was unlocked.
            if (old_waiters != QMutexPrivate::BigNumber) {
                //we did not break the previous loop
                Q_ASSERT(d->waiters.load() >= 1);
                d->waiters.deref();
            }
            d->deref();
            continue;
        }

        if (d->wait(timeout)) {
            if (d->possiblyUnlocked.load() && d->possiblyUnlocked.testAndSetRelaxed(true, false))
                d->deref();
            d->derefWaiters(1);
            //we got the lock. (do not deref)
            Q_ASSERT(d == d_ptr.load());
            return true;
        } else {
            Q_ASSERT(timeout >= 0);
            //timeout
            d->derefWaiters(1);
            //There may be a race in which the mutex is unlocked right after we timed out,
            // and before we deref the waiters, so maybe the mutex is actually unlocked.
            if (!d->possiblyUnlocked.testAndSetRelaxed(false, true))
                d->deref();
            return false;
        }
    }
    Q_ASSERT(d_ptr.load() != 0);
    return true;
}

/*!
    \internal
*/
void QBasicMutex::unlockInternal()
{
    QMutexData *copy = d_ptr.loadAcquire();
    Q_ASSERT(copy); //we must be locked
    Q_ASSERT(copy != dummyLocked()); // testAndSetRelease(dummyLocked(), 0) failed

    if (copy->recursive) {
        static_cast<QRecursiveMutexPrivate *>(copy)->unlock();
        return;
    }

    QMutexPrivate *d = reinterpret_cast<QMutexPrivate *>(copy);

    if (d->waiters.fetchAndAddRelease(-QMutexPrivate::BigNumber) == 0) {
        //there is no one waiting on this mutex anymore, set the mutex as unlocked (d = 0)
        if (d_ptr.testAndSetRelease(d, 0)) {
            if (d->possiblyUnlocked.load() && d->possiblyUnlocked.testAndSetRelaxed(true, false))
                d->deref();
        }
        d->derefWaiters(0);
    } else {
        d->derefWaiters(0);
        //there are thread waiting, transfer the lock.
        d->wakeUp();
    }
    d->deref();
}

//The freelist managment
namespace {
struct FreeListConstants : QFreeListDefaultConstants {
    enum { BlockCount = 4, MaxIndex=0xffff };
    static const int Sizes[BlockCount];
};
const int FreeListConstants::Sizes[FreeListConstants::BlockCount] = {
    16,
    128,
    1024,
    FreeListConstants::MaxIndex - (16-128-1024)
};

typedef QFreeList<QMutexPrivate, FreeListConstants> FreeList;
Q_GLOBAL_STATIC(FreeList, freelist);
}

QMutexPrivate *QMutexPrivate::allocate()
{
    int i = freelist()->next();
    QMutexPrivate *d = &(*freelist())[i];
    d->id = i;
    Q_ASSERT(d->refCount.load() == 0);
    Q_ASSERT(!d->recursive);
    Q_ASSERT(!d->possiblyUnlocked.load());
    Q_ASSERT(d->waiters.load() == 0);
    d->refCount.store(1);
    return d;
}

void QMutexPrivate::release()
{
    Q_ASSERT(!recursive);
    Q_ASSERT(refCount.load() == 0);
    Q_ASSERT(!possiblyUnlocked.load());
    Q_ASSERT(waiters.load() == 0);
    freelist()->release(id);
}

// atomically substract "value" to the waiters, and remove the QMutexPrivate::BigNumber flag
void QMutexPrivate::derefWaiters(int value)
{
    int old_waiters;
    int new_waiters;
    do {
        old_waiters = waiters.load();
        new_waiters = old_waiters;
        if (new_waiters < 0) {
            new_waiters += QMutexPrivate::BigNumber;
        }
        new_waiters -= value;
    } while (!waiters.testAndSetRelaxed(old_waiters, new_waiters));
}
#endif

/*!
   \internal
 */
bool QRecursiveMutexPrivate::lock(int timeout) {
    Qt::HANDLE self = QThread::currentThreadId();
    if (owner == self) {
        ++count;
        Q_ASSERT_X(count != 0, "QMutex::lock", "Overflow in recursion counter");
        return true;
    }
    bool success = true;
    if (timeout == -1) {
        mutex.lock();
    } else {
        success = mutex.tryLock(timeout);
    }

    if (success)
        owner = self;
    return success;
}

/*!
   \internal
 */
void QRecursiveMutexPrivate::unlock()
{
    if (count > 0) {
        count--;
    } else {
        owner = 0;
        mutex.unlock();
    }
}

QT_END_NAMESPACE

