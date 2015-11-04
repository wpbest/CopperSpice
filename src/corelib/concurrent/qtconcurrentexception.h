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

#ifndef QTCONCURRENTEXCEPTION_H
#define QTCONCURRENTEXCEPTION_H

#include <QtCore/qglobal.h>
#include <QtCore/qatomic.h>
#include <exception>

QT_BEGIN_NAMESPACE

namespace QtConcurrent
{

class Q_CORE_EXPORT Exception : public std::exception
{
public:
    virtual void raise() const;
    virtual Exception *clone() const;
};

class Q_CORE_EXPORT UnhandledException : public Exception
{
public:
    void raise() const;
    Exception *clone() const;
};

namespace internal {

class Base;
class ExceptionHolder
{
public:
    ExceptionHolder(Exception *exception = 0);
    ExceptionHolder(const ExceptionHolder &other);
    void operator=(const ExceptionHolder &other);
    ~ExceptionHolder();
    Exception *exception() const;
    Base *base;
};

class Q_CORE_EXPORT ExceptionStore
{
public:
    void setException(const Exception &e);
    bool hasException() const;
    ExceptionHolder exception();
    void throwPossibleException();
    bool hasThrown() const;
    ExceptionHolder exceptionHolder;
};

} // namespace internal


} // namespace QtConcurrent

QT_END_NAMESPACE

#endif
