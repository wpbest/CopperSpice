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

#ifndef QATOMIC_BFIN_H
#define QATOMIC_BFIN_H

QT_BEGIN_NAMESPACE

#define Q_ATOMIC_INT_REFERENCE_COUNTING_IS_NOT_NATIVE

inline bool QBasicAtomicInt::isReferenceCountingNative()
{ return false; }
inline bool QBasicAtomicInt::isReferenceCountingWaitFree()
{ return false; }

#define Q_ATOMIC_INT_TEST_AND_SET_IS_NOT_NATIVE

inline bool QBasicAtomicInt::isTestAndSetNative()
{ return false; }
inline bool QBasicAtomicInt::isTestAndSetWaitFree()
{ return false; }

#define Q_ATOMIC_INT_FETCH_AND_STORE_IS_NOT_NATIVE

inline bool QBasicAtomicInt::isFetchAndStoreNative()
{ return false; }
inline bool QBasicAtomicInt::isFetchAndStoreWaitFree()
{ return false; }

#define Q_ATOMIC_INT_FETCH_AND_ADD_IS_NOT_NATIVE

inline bool QBasicAtomicInt::isFetchAndAddNative()
{ return false; }
inline bool QBasicAtomicInt::isFetchAndAddWaitFree()
{ return false; }

#define Q_ATOMIC_POINTER_TEST_AND_SET_IS_NOT_NATIVE

template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::isTestAndSetNative()
{ return false; }
template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::isTestAndSetWaitFree()
{ return false; }

#define Q_ATOMIC_POINTER_FETCH_AND_STORE_IS_NOT_NATIVE

template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::isFetchAndStoreNative()
{ return false; }
template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::isFetchAndStoreWaitFree()
{ return false; }

#define Q_ATOMIC_POINTER_FETCH_AND_ADD_IS_NOT_NATIVE

template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::isFetchAndAddNative()
{ return false; }
template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::isFetchAndAddWaitFree()
{ return false; }

#if defined(Q_OS_LINUX) && defined(Q_CC_GNU)

QT_BEGIN_INCLUDE_NAMESPACE
#include <asm/fixed_code.h>
QT_END_INCLUDE_NAMESPACE

inline bool QBasicAtomicInt::ref()
{
    int ret;
    asm volatile("R0 = 1;\n\t"
		 "P0 = %3;\n\t"
                 "CALL (%2);\n\t"
                 "%0 = R0;"
                 : "=da" (ret), "=m" (_q_value)
                 : "a" (ATOMIC_ADD32), "da" (&_q_value), "m" (_q_value)
                 : "R0", "R1", "P0", "RETS", "memory");
    return ret != 0;
}

inline bool QBasicAtomicInt::deref()
{
    int ret;
    asm volatile("R0 = 1;\n\t"
		 "P0 = %3;\n\t"
                 "CALL (%2);\n\t"
                 "%0 = R0;"
                 : "=da" (ret), "=m" (_q_value)
                 : "a" (ATOMIC_SUB32), "da" (&_q_value), "m" (_q_value)
                 : "R0", "R1", "P0", "RETS", "memory");
    return ret != 0;
}

inline bool QBasicAtomicInt::testAndSetOrdered(int expectedValue, int newValue)
{
    long int readval;
    asm volatile ("P0 = %2;\n\t"
		  "R1 = %3;\n\t"
		  "R2 = %4;\n\t"
		  "CALL (%5);\n\t"
		  "%0 = R0;\n\t"
		  : "=da" (readval), "=m" (_q_value)
		  : "da" (&_q_value),
		  "da" (expectedValue),
		  "da" (newValue),
		  "a" (ATOMIC_CAS32),
		  "m" (_q_value)
		  : "P0", "R0", "R1", "R2", "RETS", "memory", "cc");
    return readval == expectedValue;
}

inline int QBasicAtomicInt::fetchAndStoreOrdered(int newValue)
{
    asm volatile("R1 = %2;\n\t"
		 "P0 = %4;\n\t"
                 "CALL (%3);\n\t"
                 "%0 = R0;"
                 : "=da" (newValue), "=m" (_q_value)
                 : "da" (newValue), "a" (ATOMIC_XCHG32), "da" (&_q_value), "m" (_q_value)
                 : "R0", "R1", "P0", "RETS", "memory");
    return newValue;
}

inline int QBasicAtomicInt::fetchAndAddOrdered(int valueToAdd)
{
    int ret;
    asm volatile("R0 = %[val];\n\t"
		 "P0 = %[qvalp];\n\t"
                 "CALL (%[addr]);\n\t"
                 "%[ret] = R1;"
                 : [ret] "=da" (ret), "=m" (_q_value)
                 : [addr] "a" (ATOMIC_ADD32), [qvalp] "da" (&_q_value), "m" (_q_value), [val] "da" (valueToAdd)
                 : "R0", "R1", "P0", "RETS", "memory");
    return ret;
}

template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::testAndSetOrdered(T *expectedValue, T *newValue)
{
    T *readval;
    asm volatile ("P0 = %2;\n\t"
		  "R1 = %3;\n\t"
		  "R2 = %4;\n\t"
		  "CALL (%5);\n\t"
		  "%0 = R0;\n\t"
		  : "=da" (readval), "=m" (_q_value)
		  : "da" (&_q_value),
		  "da" (expectedValue),
		  "da" (newValue),
		  "a" (ATOMIC_CAS32),
		  "m" (_q_value)
		  : "P0", "R0", "R1", "R2", "RETS", "memory", "cc");
    return readval == expectedValue;
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndStoreOrdered(T *newValue)
{
    asm volatile("R1 = %2;\n\t"
		 "P0 = %4;\n\t"
                 "CALL (%3);\n\t"
                 "%0 = R0;"
                 : "=da" (newValue), "=m" (_q_value)
                 : "da" (newValue), "a" (ATOMIC_XCHG32), "da" (&_q_value), "m" (_q_value)
                 : "R0", "R1", "P0", "RETS", "memory");
    return newValue;
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndAddOrdered(qptrdiff valueToAdd)
{
    T* ret;
    asm volatile("R0 = %[val];\n\t"
		 "P0 = %[qvalp];\n\t"
                 "CALL (%[addr]);\n\t"
                 "%[ret] = R1;"
                 : [ret] "=da" (ret), "=m" (_q_value)
                 : [addr] "a" (ATOMIC_ADD32), [qvalp] "da" (&_q_value), "m" (_q_value), [val] "da" (valueToAdd * sizeof(T))
                 : "R0", "R1", "P0", "RETS", "memory");
    return ret;
}


#endif // Q_OS_LINUX && Q_CC_GNU

// Test and set for integers

inline bool QBasicAtomicInt::testAndSetRelaxed(int expectedValue, int newValue)
{
    return testAndSetOrdered(expectedValue, newValue);
}

inline bool QBasicAtomicInt::testAndSetAcquire(int expectedValue, int newValue)
{
    return testAndSetOrdered(expectedValue, newValue);
}

inline bool QBasicAtomicInt::testAndSetRelease(int expectedValue, int newValue)
{
    return testAndSetOrdered(expectedValue, newValue);
}

// Fetch and store for integers

inline int QBasicAtomicInt::fetchAndStoreRelaxed(int newValue)
{
    return fetchAndStoreOrdered(newValue);
}

inline int QBasicAtomicInt::fetchAndStoreAcquire(int newValue)
{
    return fetchAndStoreOrdered(newValue);
}

inline int QBasicAtomicInt::fetchAndStoreRelease(int newValue)
{
    return fetchAndStoreOrdered(newValue);
}

// Fetch and add for integers

inline int QBasicAtomicInt::fetchAndAddRelaxed(int valueToAdd)
{
    return fetchAndAddOrdered(valueToAdd);
}

inline int QBasicAtomicInt::fetchAndAddAcquire(int valueToAdd)
{
    return fetchAndAddOrdered(valueToAdd);
}

inline int QBasicAtomicInt::fetchAndAddRelease(int valueToAdd)
{
    return fetchAndAddOrdered(valueToAdd);
}

// Test and set for pointers

template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::testAndSetRelaxed(T *expectedValue, T *newValue)
{
    return testAndSetOrdered(expectedValue, newValue);
}

template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::testAndSetAcquire(T *expectedValue, T *newValue)
{
    return testAndSetOrdered(expectedValue, newValue);
}

template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::testAndSetRelease(T *expectedValue, T *newValue)
{
    return testAndSetOrdered(expectedValue, newValue);
}

// Fetch and store for pointers

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndStoreRelaxed(T *newValue)
{
    return fetchAndStoreOrdered(newValue);
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndStoreAcquire(T *newValue)
{
    return fetchAndStoreOrdered(newValue);
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndStoreRelease(T *newValue)
{
    return fetchAndStoreOrdered(newValue);
}

// Fetch and add for pointers

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndAddRelaxed(qptrdiff valueToAdd)
{
    return fetchAndAddOrdered(valueToAdd);
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndAddAcquire(qptrdiff valueToAdd)
{
    return fetchAndAddOrdered(valueToAdd);
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndAddRelease(qptrdiff valueToAdd)
{
    return fetchAndAddOrdered(valueToAdd);
}

QT_END_NAMESPACE

#endif // QATOMIC_BFIN_H
