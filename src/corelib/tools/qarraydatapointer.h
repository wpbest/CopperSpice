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

#ifndef QARRAYDATAPOINTER_H
#define QARRAYDATAPOINTER_H

#include <QtCore/qarraydataops.h>

QT_BEGIN_NAMESPACE

template <class T>
struct QArrayDataPointer
{
private:
    typedef QTypedArrayData<T> Data;
    typedef QArrayDataOps<T> DataOps;

public:
    QArrayDataPointer()
        : d(Data::sharedNull())
    {
    }

    QArrayDataPointer(const QArrayDataPointer &other)
        : d(other.d->ref.ref()
            ? other.d
            : other.clone(other.d->cloneFlags()))
    {
    }

    explicit QArrayDataPointer(QTypedArrayData<T> *ptr)
        : d(ptr)
    {
    }

    QArrayDataPointer(QArrayDataPointerRef<T> ref)
        : d(ref.ptr)
    {
    }

    QArrayDataPointer &operator=(const QArrayDataPointer &other)
    {
        QArrayDataPointer tmp(other);
        this->swap(tmp);
        return *this;
    }

#ifdef Q_COMPILER_RVALUE_REFS
    QArrayDataPointer(QArrayDataPointer &&other)
        : d(other.d)
    {
        other.d = Data::sharedNull();
    }

    QArrayDataPointer &operator=(QArrayDataPointer &&other)
    {
        this->swap(other);
        return *this;
    }
#endif

    DataOps &operator*() const
    {
        Q_ASSERT(d);
        return *static_cast<DataOps *>(d);
    }

    DataOps *operator->() const
    {
        Q_ASSERT(d);
        return static_cast<DataOps *>(d);
    }

    ~QArrayDataPointer()
    {
        if (!d->ref.deref()) {
            (*this)->destroyAll();
            Data::deallocate(d);
        }
    }

    bool isNull() const
    {
        return d == Data::sharedNull();
    }

    Data *data() const
    {
        return d;
    }

    bool needsDetach() const
    {
        return (!d->isMutable() || d->ref.isShared());
    }

    void setSharable(bool sharable)
    {
        if (needsDetach()) {
            Data *detached = clone(sharable
                    ? d->detachFlags() & ~QArrayData::Unsharable
                    : d->detachFlags() | QArrayData::Unsharable);
            QArrayDataPointer old(d);
            d = detached;
        } else {
            d->ref.setSharable(sharable);
        }
    }

    void swap(QArrayDataPointer &other)
    {
        qSwap(d, other.d);
    }

    void clear()
    {
        QArrayDataPointer tmp(d);
        d = Data::allocate(0);
    }

    bool detach()
    {
<<<<<<< HEAD
        if (needsDetach()) {
=======
        if (d->ref.isShared()) {
>>>>>>> f1e48d4... Add AllocateOptions to QArrayData
            Data *copy = clone(d->detachFlags());
            QArrayDataPointer old(d);
            d = copy;
            return true;
        }

        return false;
    }

private:
<<<<<<< HEAD
    Data *clone(QArrayData::AllocationOptions options) const Q_REQUIRED_RESULT
    {
        QArrayDataPointer copy(Data::allocate(d->detachCapacity(d->size),
=======
    Data *clone(QArrayData::AllocateOptions options) const Q_REQUIRED_RESULT
    {
        QArrayDataPointer copy(Data::allocate(d->alloc ? d->alloc : d->size,
>>>>>>> f1e48d4... Add AllocateOptions to QArrayData
                    options));
        if (d->size)
            copy->copyAppend(d->begin(), d->end());

        Data *result = copy.d;
        copy.d = Data::sharedNull();
        return result;
    }

    Data *d;
};

template <class T>
inline bool operator==(const QArrayDataPointer<T> &lhs, const QArrayDataPointer<T> &rhs)
{
    return lhs.data() == rhs.data();
}

template <class T>
inline bool operator!=(const QArrayDataPointer<T> &lhs, const QArrayDataPointer<T> &rhs)
{
    return lhs.data() != rhs.data();
}

template <class T>
inline void qSwap(QArrayDataPointer<T> &p1, QArrayDataPointer<T> &p2)
{
    p1.swap(p2);
}

QT_END_NAMESPACE

namespace std
{
    template <class T>
    inline void swap(
            QT_PREPEND_NAMESPACE(QArrayDataPointer)<T> &p1,
            QT_PREPEND_NAMESPACE(QArrayDataPointer)<T> &p2)
    {
        p1.swap(p2);
    }
}


#endif // include guard
