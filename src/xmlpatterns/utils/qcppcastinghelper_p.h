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

#ifndef Patternist_CppCastingHelper_P_H
#define Patternist_CppCastingHelper_P_H

#include <QtCore/QtGlobal>

QT_BEGIN_NAMESPACE

namespace QPatternist
{   
    template<typename TSubClass>
    class CppCastingHelper
    {
    public:

        /**
         * Casts this instance to:
         *
         * @code
         * const TCastTarget *
         * @endcode
         *
         * and returns the result.
         *
         * When compiled in debug mode, this function perform a @c dynamic_cast, in order to
         * check the correctness of the cast.
         */
        template<typename TCastTarget>
        inline const TCastTarget *as() const
        {
#if defined(Patternist_DEBUG) && !defined(Q_CC_XLC)
/* At least on aix-xlc-64, the compiler cries when it sees dynamic_cast. */
            Q_ASSERT_X(dynamic_cast<const TCastTarget *>(static_cast<const TSubClass *>(this)),
                       Q_FUNC_INFO,
                       "The cast is invalid. This class does not inherit the cast target.");
#endif
            return static_cast<const TCastTarget *>(static_cast<const TSubClass *>(this));
        }

        /**
         * Casts this instance to:
         *
         * @code
         * TCastTarget *
         * @endcode
         *
         * and returns the result.
         *
         * When compiled in debug mode, a @c dynamic_cast is attempted, in order to
         * check the correctness of the cast.
         */
        template<typename TCastTarget>
        inline TCastTarget *as()
        {

#if defined(Patternist_DEBUG) && !defined(Q_CC_XLC)
/* on aix-xlc-64, the compiler cries when it sees dynamic_cast. */
            Q_ASSERT_X(dynamic_cast<TCastTarget *>(static_cast<TSubClass *>(this)),
                       Q_FUNC_INFO, "The cast is invalid. This class does not inherit the cast target.");
#endif
            return static_cast<TCastTarget *>(static_cast<TSubClass *>(this));
        }

    protected:
        /**
         * This constructor is protected because this class must be sub-classed.
         */
        inline CppCastingHelper() {}
    };
}

QT_END_NAMESPACE

#endif
