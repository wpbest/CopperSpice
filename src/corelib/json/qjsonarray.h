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

#ifndef QJSONARRAY_H
#define QJSONARRAY_H

#include <QtCore/qjsonvalue.h>
#include <QtCore/qiterator.h>

QT_BEGIN_NAMESPACE

class QDebug;
class QStringList;
template <typename T> class QList;
typedef QList<QVariant> QVariantList;

class Q_CORE_EXPORT QJsonArray
{
public:
    QJsonArray();
    ~QJsonArray();

    QJsonArray(const QJsonArray &other);
    QJsonArray &operator =(const QJsonArray &other);

    static QJsonArray fromStringList(const QStringList &list);
    static QJsonArray fromVariantList(const QVariantList &list);
    QVariantList toVariantList() const;

    int size() const;
    inline int count() const { return size(); }

    bool isEmpty() const;
    QJsonValue at(int i) const;
    QJsonValue first() const;
    QJsonValue last() const;

    void prepend(const QJsonValue &value);
    void append(const QJsonValue &value);
    void removeAt(int i);
    QJsonValue takeAt(int i);
    inline void removeFirst() { removeAt(0); }
    inline void removeLast() { removeAt(size() - 1); }

    void insert(int i, const QJsonValue &value);
    void replace(int i, const QJsonValue &value);

    bool contains(const QJsonValue &element) const;
    QJsonValueRef operator[](int i);
    QJsonValue operator[](int i) const;

    bool operator==(const QJsonArray &other) const;
    bool operator!=(const QJsonArray &other) const;

    class const_iterator;

    class iterator {
    public:
        QJsonArray *a;
        int i;
        typedef std::random_access_iterator_tag  iterator_category;
        typedef int difference_type;
        typedef QJsonValue value_type;
        //typedef T *pointer;
        typedef QJsonValueRef reference;

        inline iterator() : a(0), i(0) { }
        explicit inline iterator(QJsonArray *array, int index) : a(array), i(index) { }

        inline QJsonValueRef operator*() const { return QJsonValueRef(a, i); }
        //inline T *operator->() const { return &concrete(i)->value; }
        inline QJsonValueRef operator[](int j) const { return QJsonValueRef(a, i + j); }

        inline bool operator==(const iterator &o) const { return i == o.i; }
        inline bool operator!=(const iterator &o) const { return i != o.i; }
        inline bool operator<(const iterator& other) const { return i < other.i; }
        inline bool operator<=(const iterator& other) const { return i <= other.i; }
        inline bool operator>(const iterator& other) const { return i > other.i; }
        inline bool operator>=(const iterator& other) const { return i >= other.i; }
        inline bool operator==(const const_iterator &o) const { return i == o.i; }
        inline bool operator!=(const const_iterator &o) const { return i != o.i; }
        inline bool operator<(const const_iterator& other) const { return i < other.i; }
        inline bool operator<=(const const_iterator& other) const { return i <= other.i; }
        inline bool operator>(const const_iterator& other) const { return i > other.i; }
        inline bool operator>=(const const_iterator& other) const { return i >= other.i; }
        inline iterator &operator++() { ++i; return *this; }
        inline iterator operator++(int) { iterator n = *this; ++i; return n; }
        inline iterator &operator--() { i--; return *this; }
        inline iterator operator--(int) { iterator n = *this; i--; return n; }
        inline iterator &operator+=(int j) { i+=j; return *this; }
        inline iterator &operator-=(int j) { i-=j; return *this; }
        inline iterator operator+(int j) const { return iterator(a, i+j); }
        inline iterator operator-(int j) const { return iterator(a, i-j); }
        inline int operator-(iterator j) const { return i - j.i; }
    };
    friend class iterator;

    class const_iterator {
    public:
        const QJsonArray *a;
        int i;
        typedef std::random_access_iterator_tag  iterator_category;
        typedef qptrdiff difference_type;
        typedef QJsonValue value_type;
        //typedef const T *pointer;
        typedef QJsonValue reference;

        inline const_iterator() : a(0), i(0) { }
        explicit inline const_iterator(const QJsonArray *array, int index) : a(array), i(index) { }
        inline const_iterator(const const_iterator &o) : a(o.a), i(o.i) {}
        inline const_iterator(const iterator &o) : a(o.a), i(o.i) {}

        inline QJsonValue operator*() const { return a->at(i); }
        //inline T *operator->() const { return &concrete(i)->value; }
        inline QJsonValue operator[](int j) const { return a->at(i+j); }
        inline bool operator==(const const_iterator &o) const { return i == o.i; }
        inline bool operator!=(const const_iterator &o) const { return i != o.i; }
        inline bool operator<(const const_iterator& other) const { return i < other.i; }
        inline bool operator<=(const const_iterator& other) const { return i <= other.i; }
        inline bool operator>(const const_iterator& other) const { return i > other.i; }
        inline bool operator>=(const const_iterator& other) const { return i >= other.i; }
        inline const_iterator &operator++() { ++i; return *this; }
        inline const_iterator operator++(int) { const_iterator n = *this; ++i; return n; }
        inline const_iterator &operator--() { i--; return *this; }
        inline const_iterator operator--(int) { const_iterator n = *this; i--; return n; }
        inline const_iterator &operator+=(int j) { i+=j; return *this; }
        inline const_iterator &operator-=(int j) { i-=j; return *this; }
        inline const_iterator operator+(int j) const { return const_iterator(a, i+j); }
        inline const_iterator operator-(int j) const { return const_iterator(a, i-j); }
        inline int operator-(const_iterator j) const { return i - j.i; }
    };
    friend class const_iterator;

    // stl style
    inline iterator begin() { detach(); return iterator(this, 0); }
    inline const_iterator begin() const { return const_iterator(this, 0); }
    inline const_iterator constBegin() const { return const_iterator(this, 0); }
    inline iterator end() { detach(); return iterator(this, size()); }
    inline const_iterator end() const { return const_iterator(this, size()); }
    inline const_iterator constEnd() const { return const_iterator(this, size()); }
    iterator insert(iterator before, const QJsonValue &value) { insert(before.i, value); return before; }
    iterator erase(iterator it) { removeAt(it.i); return it; }

    // more Qt
    typedef iterator Iterator;
    typedef const_iterator ConstIterator;

    // stl compatibility
    inline void push_back(const QJsonValue &t) { append(t); }
    inline void push_front(const QJsonValue &t) { prepend(t); }
    inline void pop_front() { removeFirst(); }
    inline void pop_back() { removeLast(); }
    inline bool empty() const { return isEmpty(); }
    typedef int size_type;
    typedef QJsonValue value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef QJsonValueRef reference;
    typedef QJsonValue const_reference;
    typedef int difference_type;

private:
    friend class QJsonPrivate::Data;
    friend class QJsonValue;
    friend class QJsonDocument;
    friend Q_CORE_EXPORT QDebug operator<<(QDebug, const QJsonArray &);

    QJsonArray(QJsonPrivate::Data *data, QJsonPrivate::Array *array);
    void compact();
    void detach(uint reserve = 0);

    QJsonPrivate::Data *d;
    QJsonPrivate::Array *a;
};

#ifndef QT_NO_DEBUG_STREAM
Q_CORE_EXPORT QDebug operator<<(QDebug, const QJsonArray &);
#endif

QT_END_NAMESPACE

#endif // QJSONARRAY_H
