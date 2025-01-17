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

#ifndef QATOMIC_POWERPC_H
#define QATOMIC_POWERPC_H

QT_BEGIN_NAMESPACE

#define Q_ATOMIC_INT_REFERENCE_COUNTING_IS_ALWAYS_NATIVE

inline bool QBasicAtomicInt::isReferenceCountingNative()
{ return true; }
inline bool QBasicAtomicInt::isReferenceCountingWaitFree()
{ return false; }

#define Q_ATOMIC_INT_TEST_AND_SET_IS_ALWAYS_NATIVE

inline bool QBasicAtomicInt::isTestAndSetNative()
{ return true; }
inline bool QBasicAtomicInt::isTestAndSetWaitFree()
{ return false; }

#define Q_ATOMIC_INT_FETCH_AND_STORE_IS_ALWAYS_NATIVE

inline bool QBasicAtomicInt::isFetchAndStoreNative()
{ return true; }
inline bool QBasicAtomicInt::isFetchAndStoreWaitFree()
{ return false; }

#define Q_ATOMIC_INT_FETCH_AND_ADD_IS_ALWAYS_NATIVE

inline bool QBasicAtomicInt::isFetchAndAddNative()
{ return true; }
inline bool QBasicAtomicInt::isFetchAndAddWaitFree()
{ return false; }

#define Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE

template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::isTestAndSetNative()
{ return true; }
template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::isTestAndSetWaitFree()
{ return false; }

#define Q_ATOMIC_POINTER_FETCH_AND_STORE_IS_ALWAYS_NATIVE

template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::isFetchAndStoreNative()
{ return true; }
template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::isFetchAndStoreWaitFree()
{ return false; }

#define Q_ATOMIC_POINTER_FETCH_AND_ADD_IS_ALWAYS_NATIVE

template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::isFetchAndAddNative()
{ return true; }
template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::isFetchAndAddWaitFree()
{ return false; }

#if defined(Q_CC_GNU)

#if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 2) \
    || (!defined(__64BIT__) && !defined(__powerpc64__) && !defined(__ppc64__))
#  define _Q_VALUE "0, %[_q_value]"
#  define _Q_VALUE_MEMORY_OPERAND "+m" (_q_value)
#  define _Q_VALUE_REGISTER_OPERAND [_q_value] "r" (&_q_value),
#else
// On 64-bit with gcc >= 4.2
#  define _Q_VALUE "%y[_q_value]"
#  define _Q_VALUE_MEMORY_OPERAND [_q_value] "+Z" (_q_value)
#  define _Q_VALUE_REGISTER_OPERAND
#endif

inline bool QBasicAtomicInt::ref()
{
    register int originalValue;
    register int newValue;
    asm volatile("lwarx  %[originalValue]," _Q_VALUE "\n"
                 "addi   %[newValue], %[originalValue], %[one]\n"
                 "stwcx. %[newValue]," _Q_VALUE "\n"
                 "bne-   $-12\n"
                 : [originalValue] "=&b" (originalValue),
                   [newValue] "=&r" (newValue),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [one] "i" (1)
                 : "cc", "memory");
    return newValue != 0;
}

inline bool QBasicAtomicInt::deref()
{
    register int originalValue;
    register int newValue;
    asm volatile("lwarx  %[originalValue]," _Q_VALUE "\n"
                 "addi   %[newValue], %[originalValue], %[minusOne]\n"
                 "stwcx. %[newValue]," _Q_VALUE "\n"
                 "bne-   $-12\n"
                 : [originalValue] "=&b" (originalValue),
                   [newValue] "=&r" (newValue),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [minusOne] "i" (-1)
                 : "cc", "memory");
    return newValue != 0;
}

inline bool QBasicAtomicInt::testAndSetRelaxed(int expectedValue, int newValue)
{
    register int result;
    asm volatile("lwarx  %[result]," _Q_VALUE "\n"
                 "xor.   %[result], %[result], %[expectedValue]\n"
                 "bne    $+12\n"
                 "stwcx. %[newValue]," _Q_VALUE "\n"
                 "bne-   $-16\n"
                 : [result] "=&r" (result),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [expectedValue] "r" (expectedValue),
                   [newValue] "r" (newValue)
                 : "cc", "memory");
    return result == 0;
}

inline bool QBasicAtomicInt::testAndSetAcquire(int expectedValue, int newValue)
{
    register int result;
    asm volatile("lwarx  %[result]," _Q_VALUE "\n"
                 "xor.   %[result], %[result], %[expectedValue]\n"
                 "bne    $+16\n"
                 "stwcx. %[newValue]," _Q_VALUE "\n"
                 "bne-   $-16\n"
                 "isync\n"
                 : [result] "=&r" (result),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [expectedValue] "r" (expectedValue),
                   [newValue] "r" (newValue)
                 : "cc", "memory");
    return result == 0;
}

inline bool QBasicAtomicInt::testAndSetRelease(int expectedValue, int newValue)
{
    register int result;
    asm volatile("eieio\n"
                 "lwarx  %[result]," _Q_VALUE "\n"
                 "xor.   %[result], %[result], %[expectedValue]\n"
                 "bne    $+12\n"
                 "stwcx. %[newValue]," _Q_VALUE "\n"
                 "bne-   $-16\n"
                 : [result] "=&r" (result),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [expectedValue] "r" (expectedValue),
                   [newValue] "r" (newValue)
                 : "cc", "memory");
    return result == 0;
}

inline int QBasicAtomicInt::fetchAndStoreRelaxed(int newValue)
{
    register int originalValue;
    asm volatile("lwarx  %[originalValue]," _Q_VALUE "\n"
                 "stwcx. %[newValue]," _Q_VALUE "\n"
                 "bne-   $-8\n"
                 : [originalValue] "=&r" (originalValue),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [newValue] "r" (newValue)
                 : "cc", "memory");
    return originalValue;
}

inline int QBasicAtomicInt::fetchAndStoreAcquire(int newValue)
{
    register int originalValue;
    asm volatile("lwarx  %[originalValue]," _Q_VALUE "\n"
                 "stwcx. %[newValue]," _Q_VALUE "\n"
                 "bne-   $-8\n"
                 "isync\n"
                 : [originalValue] "=&r" (originalValue),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [newValue] "r" (newValue)
                 : "cc", "memory");
    return originalValue;
}

inline int QBasicAtomicInt::fetchAndStoreRelease(int newValue)
{
    register int originalValue;
    asm volatile("eieio\n"
                 "lwarx  %[originalValue]," _Q_VALUE "\n"
                 "stwcx. %[newValue]," _Q_VALUE "\n"
                 "bne-   $-8\n"
                 : [originalValue] "=&r" (originalValue),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [newValue] "r" (newValue)
                 : "cc", "memory");
    return originalValue;
}

inline int QBasicAtomicInt::fetchAndAddRelaxed(int valueToAdd)
{
    register int originalValue;
    register int newValue;
    asm volatile("lwarx  %[originalValue]," _Q_VALUE "\n"
                 "add    %[newValue], %[originalValue], %[valueToAdd]\n"
                 "stwcx. %[newValue]," _Q_VALUE "\n"
                 "bne-   $-12\n"
                 : [originalValue] "=&r" (originalValue),
                   [newValue] "=&r" (newValue),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [valueToAdd] "r" (valueToAdd)
                 : "cc", "memory");
    return originalValue;
}

inline int QBasicAtomicInt::fetchAndAddAcquire(int valueToAdd)
{
    register int originalValue;
    register int newValue;
    asm volatile("lwarx  %[originalValue]," _Q_VALUE "\n"
                 "add    %[newValue], %[originalValue], %[valueToAdd]\n"
                 "stwcx. %[newValue]," _Q_VALUE "\n"
                 "bne-   $-12\n"
                 "isync\n"
                 : [originalValue] "=&r" (originalValue),
                   [newValue] "=&r" (newValue),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [valueToAdd] "r" (valueToAdd)
                 : "cc", "memory");
    return originalValue;
}

inline int QBasicAtomicInt::fetchAndAddRelease(int valueToAdd)
{
    register int originalValue;
    register int newValue;
    asm volatile("eieio\n"
                 "lwarx  %[originalValue]," _Q_VALUE "\n"
                 "add    %[newValue], %[originalValue], %[valueToAdd]\n"
                 "stwcx. %[newValue]," _Q_VALUE "\n"
                 "bne-   $-12\n"
                 : [originalValue] "=&r" (originalValue),
                   [newValue] "=&r" (newValue),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [valueToAdd] "r" (valueToAdd)
                 : "cc", "memory");
    return originalValue;
}

#if defined(__64BIT__) || defined(__powerpc64__) || defined(__ppc64__)
#  define LPARX "ldarx"
#  define STPCX "stdcx."
#else
#  define LPARX "lwarx"
#  define STPCX "stwcx."
#endif

template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::testAndSetRelaxed(T *expectedValue, T *newValue)
{
    register void *result;
    asm volatile(LPARX"  %[result]," _Q_VALUE "\n"
                 "xor.   %[result], %[result], %[expectedValue]\n"
                 "bne    $+12\n"
                 STPCX"  %[newValue]," _Q_VALUE "\n"
                 "bne-   $-16\n"
                 : [result] "=&r" (result),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [expectedValue] "r" (expectedValue),
                   [newValue] "r" (newValue)
                 : "cc", "memory");
    return result == 0;
}

template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::testAndSetAcquire(T *expectedValue, T *newValue)
{
    register void *result;
    asm volatile(LPARX"  %[result]," _Q_VALUE "\n"
                 "xor.   %[result], %[result], %[expectedValue]\n"
                 "bne    $+16\n"
                 STPCX"  %[newValue]," _Q_VALUE "\n"
                 "bne-   $-16\n"
                 "isync\n"
                 : [result] "=&r" (result),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [expectedValue] "r" (expectedValue),
                   [newValue] "r" (newValue)
                 : "cc", "memory");
    return result == 0;
}

template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::testAndSetRelease(T *expectedValue, T *newValue)
{
    register void *result;
    asm volatile("eieio\n"
                 LPARX"  %[result]," _Q_VALUE "\n"
                 "xor.   %[result], %[result], %[expectedValue]\n"
                 "bne    $+12\n"
                 STPCX"  %[newValue]," _Q_VALUE "\n"
                 "bne-   $-16\n"
                 : [result] "=&r" (result),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [expectedValue] "r" (expectedValue),
                   [newValue] "r" (newValue)
                 : "cc", "memory");
    return result == 0;
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndStoreRelaxed(T *newValue)
{
    register T *originalValue;
    asm volatile(LPARX"  %[originalValue]," _Q_VALUE "\n"
                 STPCX"  %[newValue]," _Q_VALUE "\n"
                 "bne-   $-8\n"
                 : [originalValue] "=&r" (originalValue),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [newValue] "r" (newValue)
                 : "cc", "memory");
    return originalValue;
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndStoreAcquire(T *newValue)
{
    register T *originalValue;
    asm volatile(LPARX"  %[originalValue]," _Q_VALUE "\n"
                 STPCX"  %[newValue]," _Q_VALUE "\n"
                 "bne-   $-8\n"
                 "isync\n"
                 : [originalValue] "=&r" (originalValue),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [newValue] "r" (newValue)
                 : "cc", "memory");
    return originalValue;
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndStoreRelease(T *newValue)
{
    register T *originalValue;
    asm volatile("eieio\n"
                 LPARX"  %[originalValue]," _Q_VALUE "\n"
                 STPCX"  %[newValue]," _Q_VALUE "\n"
                 "bne-   $-8\n"
                 : [originalValue] "=&r" (originalValue),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [newValue] "r" (newValue)
                 : "cc", "memory");
    return originalValue;
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndAddRelaxed(qptrdiff valueToAdd)
{
    register T *originalValue;
    register T *newValue;
    asm volatile(LPARX"  %[originalValue]," _Q_VALUE "\n"
                 "add    %[newValue], %[originalValue], %[valueToAdd]\n"
                 STPCX"  %[newValue]," _Q_VALUE "\n"
                 "bne-   $-12\n"
                 : [originalValue] "=&r" (originalValue),
                   [newValue] "=&r" (newValue),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [valueToAdd] "r" (valueToAdd * sizeof(T))
                 : "cc", "memory");
    return originalValue;
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndAddAcquire(qptrdiff valueToAdd)
{
    register T *originalValue;
    register T *newValue;
    asm volatile(LPARX"  %[originalValue]," _Q_VALUE "\n"
                 "add    %[newValue], %[originalValue], %[valueToAdd]\n"
                 STPCX"  %[newValue]," _Q_VALUE "\n"
                 "bne-   $-12\n"
                 "isync\n"
                 : [originalValue] "=&r" (originalValue),
                   [newValue] "=&r" (newValue),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [valueToAdd] "r" (valueToAdd * sizeof(T))
                 : "cc", "memory");
    return originalValue;
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndAddRelease(qptrdiff valueToAdd)
{
    register T *originalValue;
    register T *newValue;
    asm volatile("eieio\n"
                 LPARX"  %[originalValue]," _Q_VALUE "\n"
                 "add    %[newValue], %[originalValue], %[valueToAdd]\n"
                 STPCX"  %[newValue]," _Q_VALUE "\n"
                 "bne-   $-12\n"
                 : [originalValue] "=&r" (originalValue),
                   [newValue] "=&r" (newValue),
                   _Q_VALUE_MEMORY_OPERAND
                 : _Q_VALUE_REGISTER_OPERAND
                   [valueToAdd] "r" (valueToAdd * sizeof(T))
                 : "cc", "memory");
    return originalValue;
}

#undef LPARX
#undef STPCX
#undef _Q_VALUE
#undef _Q_VALUE_MEMORY_OPERAND
#undef _Q_VALUE_REGISTER_OPERAND

#else

extern "C" {
    int q_atomic_test_and_set_int(volatile int *ptr, int expectedValue, int newValue);
    int q_atomic_test_and_set_acquire_int(volatile int *ptr, int expectedValue, int newValue);
    int q_atomic_test_and_set_release_int(volatile int *ptr, int expectedValue, int newValue);
    int q_atomic_test_and_set_ptr(volatile void *ptr, void *expectedValue, void *newValue);
    int q_atomic_test_and_set_acquire_ptr(volatile void *ptr, void *expectedValue, void *newValue);
    int q_atomic_test_and_set_release_ptr(volatile void *ptr, void *expectedValue, void *newValue);
    int q_atomic_increment(volatile int *);
    int q_atomic_decrement(volatile int *);
    int q_atomic_set_int(volatile int *, int);
    int q_atomic_fetch_and_store_acquire_int(volatile int *ptr, int newValue);
    int q_atomic_fetch_and_store_release_int(volatile int *ptr, int newValue);
    void *q_atomic_set_ptr(volatile void *, void *);
    int q_atomic_fetch_and_store_acquire_ptr(volatile void *ptr, void *newValue);
    int q_atomic_fetch_and_store_release_ptr(volatile void *ptr, void *newValue);
    int q_atomic_fetch_and_add_int(volatile int *ptr, int valueToAdd);
    int q_atomic_fetch_and_add_acquire_int(volatile int *ptr, int valueToAdd);
    int q_atomic_fetch_and_add_release_int(volatile int *ptr, int valueToAdd);
    void *q_atomic_fetch_and_add_ptr(volatile void *ptr, qptrdiff valueToAdd);
    void *q_atomic_fetch_and_add_acquire_ptr(volatile void *ptr, qptrdiff valueToAdd);
    void *q_atomic_fetch_and_add_release_ptr(volatile void *ptr, qptrdiff valueToAdd);
} // extern "C"


inline bool QBasicAtomicInt::ref()
{
    return q_atomic_increment(&_q_value) != 0;
}

inline bool QBasicAtomicInt::deref()
{
    return q_atomic_decrement(&_q_value) != 0;
}

inline bool QBasicAtomicInt::testAndSetRelaxed(int expectedValue, int newValue)
{
    return q_atomic_test_and_set_int(&_q_value, expectedValue, newValue) != 0;
}

inline bool QBasicAtomicInt::testAndSetAcquire(int expectedValue, int newValue)
{
    return q_atomic_test_and_set_acquire_int(&_q_value, expectedValue, newValue) != 0;
}

inline bool QBasicAtomicInt::testAndSetRelease(int expectedValue, int newValue)
{
    return q_atomic_test_and_set_release_int(&_q_value, expectedValue, newValue) != 0;
}

inline int QBasicAtomicInt::fetchAndStoreRelaxed(int newValue)
{
    return q_atomic_set_int(&_q_value, newValue);
}

inline int QBasicAtomicInt::fetchAndStoreAcquire(int newValue)
{
    return q_atomic_fetch_and_store_acquire_int(&_q_value, newValue);
}

inline int QBasicAtomicInt::fetchAndStoreRelease(int newValue)
{
    return q_atomic_fetch_and_store_release_int(&_q_value, newValue);
}

inline int QBasicAtomicInt::fetchAndAddRelaxed(int valueToAdd)
{
    return q_atomic_fetch_and_add_int(&_q_value, valueToAdd);
}

inline int QBasicAtomicInt::fetchAndAddAcquire(int valueToAdd)
{
    return q_atomic_fetch_and_add_acquire_int(&_q_value, valueToAdd);
}

inline int QBasicAtomicInt::fetchAndAddRelease(int valueToAdd)
{
    return q_atomic_fetch_and_add_release_int(&_q_value, valueToAdd);
}

template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::testAndSetRelaxed(T *expectedValue, T *newValue)
{
    return q_atomic_test_and_set_ptr(&_q_value, expectedValue, newValue) != 0;
}

template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::testAndSetAcquire(T *expectedValue, T *newValue)
{
    return q_atomic_test_and_set_acquire_ptr(&_q_value, expectedValue, newValue) != 0;
}

template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::testAndSetRelease(T *expectedValue, T *newValue)
{
    return q_atomic_test_and_set_release_ptr(&_q_value, expectedValue, newValue) != 0;
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndStoreRelaxed(T *newValue)
{
    return reinterpret_cast<T *>(q_atomic_set_ptr(&_q_value, newValue));
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndStoreAcquire(T *newValue)
{
    return reinterpret_cast<T *>(q_atomic_fetch_and_store_acquire_ptr(&_q_value, newValue));
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndStoreRelease(T *newValue)
{
    return reinterpret_cast<T *>(q_atomic_fetch_and_store_release_ptr(&_q_value, newValue));
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndAddRelaxed(qptrdiff valueToAdd)
{
    return reinterpret_cast<T *>(q_atomic_fetch_and_add_ptr(&_q_value, valueToAdd * sizeof(T)));
}
template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndAddAcquire(qptrdiff valueToAdd)
{
    return reinterpret_cast<T *>(q_atomic_fetch_and_add_acquire_ptr(&_q_value, valueToAdd * sizeof(T)));
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndAddRelease(qptrdiff valueToAdd)
{
    return reinterpret_cast<T *>(q_atomic_fetch_and_add_release_ptr(&_q_value, valueToAdd * sizeof(T)));
}

#endif

inline bool QBasicAtomicInt::testAndSetOrdered(int expectedValue, int newValue)
{
    return testAndSetAcquire(expectedValue, newValue);
}

inline int QBasicAtomicInt::fetchAndStoreOrdered(int newValue)
{
    return fetchAndStoreAcquire(newValue);
}

inline int QBasicAtomicInt::fetchAndAddOrdered(int valueToAdd)
{
    return fetchAndAddAcquire(valueToAdd);
}

template <typename T>
Q_INLINE_TEMPLATE bool QBasicAtomicPointer<T>::testAndSetOrdered(T *expectedValue, T *newValue)
{
    return testAndSetAcquire(expectedValue, newValue);
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndStoreOrdered(T *newValue)
{
    return fetchAndStoreAcquire(newValue);
}

template <typename T>
Q_INLINE_TEMPLATE T *QBasicAtomicPointer<T>::fetchAndAddOrdered(qptrdiff valueToAdd)
{
    return fetchAndAddAcquire(valueToAdd);
}

QT_END_NAMESPACE

#endif // QATOMIC_POWERPC_H
